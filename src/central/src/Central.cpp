#include "Central.h"
#include "Logger.h"
#include <mutex>
#include <blocks/BlockReader.h>
#include <algorithm>
#include <blocks/CentralServerHeartbeat.h>

using namespace std;
using placeholders::_1;
using chrono::duration_cast;

Central::Central(const CentralConfig &config) : config(config) {
    logInfo() << "Central: constructor with config ["
              << "id=" << config.id << "; "
              << "port=" << config.port << "; "
              << "heartbeat_interval_ms=" << config.heartbeat_interval_ms << "; "
              << "alert_interval_ms=" << config.alert_interval_ms << "; "
              << "fire_dept_ip=" << config.fire_dept_ip << "; "
              << "fire_dept_port=" << config.fire_dept_port
              << "]";

    logDebug() << "Central: debug config read ["
              << "get_heartbeat_interval=" << get_heartbeat_interval().count() << "ms; "
              << "get_alert_interval=" << get_alert_interval().count() << "ms; "
              << "get_id=" << get_id() << "; "
              << "get_fire_dept_address=" << UdpConnection::address_to_str(get_fire_dept_address())
              << "]";
}

void Central::reload_config(const CentralConfig &config) {
    auto prev_config = this->config;

    {
        boost::shared_lock<boost::shared_mutex> lock(config_lock);
        this->config = config;
    }

    if (prev_config.port != this->config.port) {
        apply_port_change();
    }
}

void Central::shutdown() {
    logInfo() << "Central: received shutdown";
    running.store(false, memory_order_release);
}

void Central::run() {
    logInfo() << "Central: initializing";

    init_socket();
    last_heartbeat = chrono::system_clock::now() - get_heartbeat_interval();
    running.store(true, memory_order_release);
    listen_thread = thread(bind(&Central::run_listen, this));
    alert_send_thread = thread(bind(&Central::run_sending_alerts, this));

    logInfo() << "Central: initialized";

    run_send_heartbeats();

    listen_thread.join();
    alert_send_thread.join();
}

void Central::run_send_heartbeats() {
    logInfo() << "Central: start sending heartbeats";

    while (running.load(memory_order_consume)) {
        const auto since_last_heartbeat = chrono::_V2::system_clock::now() - last_heartbeat;
        const chrono::milliseconds heartbeat_interval = get_heartbeat_interval();

        logDebug() << "Central: before send heartbeat: ["
                   << "since_last_heartbeat=" << duration_cast<chrono::milliseconds>(since_last_heartbeat).count()
                   << "ms; heartbeat_interval=" << duration_cast<chrono::milliseconds>(heartbeat_interval).count()
                   << "ms]";

        if (since_last_heartbeat >= heartbeat_interval) {
            send_heartbeat();
        } else {
            logInfo() << "Central: ignoring heartbeat";
        }

        const auto wait_duration = heartbeat_interval - since_last_heartbeat;

        logDebug() << "Central: waiting " << duration_cast<chrono::milliseconds>(wait_duration).count()
                  << "ms for next heartbeat";
        this_thread::sleep_for(wait_duration);
    }

    logInfo() << "Central: stop sending heartbeats";
}

void Central::run_listen() {
    logInfo() << "Central: start listening";

    const std::size_t buffer_size = 512;
    uint8_t buffer[buffer_size];

    while (running.load(memory_order_consume)) {
        std::size_t bytes_read;
        sockaddr_storage addr;

        {
            boost::shared_lock<boost::shared_mutex> lock(connection_lock);
            addr = connection.receive(buffer, buffer_size, bytes_read);
        }

        try {
            logDebug() << "Central: deserializing message from " << connection.address_to_str(addr);

            const auto blocks = move(BlockReader(buffer, bytes_read).blocks);
            const auto common = find_if(blocks.begin(), blocks.end(),
                    bind(equal_to<BlockType>(), std::bind(&AbstractBlock::type, _1), BlockType::sensor_common)
            );

            if (common != blocks.end()) {
                auto common_ref = dynamic_cast<const SensorCommonBlock&>(**common);
                for_each(blocks.begin(), blocks.end(), bind(&Central::handle_block, this, common_ref, _1));
            } else {
                throw runtime_error("message does not contain common sensor block");
            }

        } catch (const std::exception &e) {
            logError() << "Central: Failed to parse message from "
                       << connection.address_to_str(addr) << ": " << e.what();
        } catch (...) {
            logError() << "Central: Unknown error occurred while parsing message from "
                       << connection.address_to_str(addr);
        }
    }

    logInfo() << "Central: stop listening";
}

void Central::run_sending_alerts() {
    logInfo() << "Central: start sending alerts";

    Serializer serializer;

    while (running.load(memory_order_consume)) {
        auto alerts = alert_aggregator.extractAlerts();
        if (!alerts.empty()) {
            logDebug() << "Central: sending " << alerts.size() << " alerts";

            serializer.clear();
            for_each(alerts.begin(), alerts.end(), bind(&AbstractBlock::serialize, _1, ref(serializer)));

            uint16_t buffer_size;
            const uint8_t *buffer = serializer.get_buffer(buffer_size);

            logDebug() << "Central: alerts buffer_size=" << buffer_size;

            boost::shared_lock<boost::shared_mutex> lock(connection_lock);
            connection.send_msg(buffer, buffer_size, get_fire_dept_address());
        }

        const chrono::milliseconds wait_duration = get_alert_interval();
        logDebug() << "Central: waiting " << wait_duration.count() << "ms for next alerts send";
        this_thread::sleep_for(wait_duration);
    }

    logInfo() << "Central: stop sending alerts";
}

void Central::handle_block(const SensorCommonBlock &common, const std::unique_ptr<AbstractBlock> &block) {
    switch (block->type) {
        case BlockType::ir_read:
        case BlockType::smoke_read:
        case BlockType::temp_read:
            alert_aggregator.insert(common, dynamic_cast<const SensorMeasurementBlock&>(*block));
            break;

        case BlockType::sensor_common:
            break;

        default:
            logWarn() << "Central: handle_block: invalid block type: " << block->type;
    }
}

void Central::send_heartbeat() {
    logDebug() << "Central: sending hearbeat";

    Serializer serializer;
    CentralServerHeartbeat(get_id()).serialize(serializer);

    uint16_t buffer_size;
    const uint8_t* buffer = serializer.get_buffer(buffer_size);

    boost::shared_lock<boost::shared_mutex> lock(connection_lock);
    connection.send_msg(buffer, buffer_size, get_fire_dept_address());
    last_heartbeat = chrono::system_clock::now();
}

void Central::init_socket() {
    boost::unique_lock<boost::shared_mutex> lock(connection_lock);
    init_socket_impl();
}

void Central::init_socket_impl() {
    try {
        logInfo() << "Central: initializing socket";
        connection.open_socket();
        connection.bind_port(config.port);
    } catch (const exception &e) {
        logError() << "Central: Exception occurred when initializing socket: " << e.what();
        throw;
    }
}

void Central::apply_port_change() {
    logInfo() << "Central: apply_port_change";
    boost::unique_lock<boost::shared_mutex> lock(connection_lock);

    connection.close_socket();
    init_socket_impl();
}

std::chrono::milliseconds Central::get_heartbeat_interval() {
    boost::shared_lock<boost::shared_mutex> lock(config_lock);
    return chrono::milliseconds(config.heartbeat_interval_ms);
}

std::chrono::milliseconds Central::get_alert_interval() {
    boost::shared_lock<boost::shared_mutex> lock(config_lock);
    return chrono::milliseconds(config.alert_interval_ms);
}

int32_t Central::get_id() {
    boost::shared_lock<boost::shared_mutex> lock(config_lock);
    return config.id;
}

sockaddr_storage Central::get_fire_dept_address() {
    boost::shared_lock<boost::shared_mutex> lock(config_lock);
    return UdpConnection::get_address(config.fire_dept_ip, config.fire_dept_port);
}

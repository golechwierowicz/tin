#include "Central.h"
#include "Logger.h"
#include <mutex>
#include <blocks/BlockReader.h>
#include <algorithm>
#include <blocks/CentralServerHeartbeat.h>

using namespace std;
using placeholders::_1;

Central::Central(const CentralConfig &config) : config(config) {
}

void Central::reload_config(const CentralConfig &config) {
    auto prev_config = this->config;

    {
        shared_lock<shared_mutex> lock(config_lock);
        this->config = config;
    }

    if (prev_config.port != this->config.port) {
        apply_port_change();
    }
}

void Central::shutdown() volatile {
    running.store(false, memory_order_release);
}

void Central::run() {
    init_socket();
    last_heartbeat = std::chrono::time_point<chrono::system_clock>::min();
    running.store(true, memory_order_release);
    listen_thread = thread(bind(&Central::listen, this));

    while (running.load(memory_order_consume)) {
        const auto since_last_heartbeat = std::chrono::system_clock::now() - last_heartbeat;
        const chrono::milliseconds heartbeat_interval = get_heartbeat_interval();

        if (since_last_heartbeat >= heartbeat_interval) {
            send_heartbeat();
        }

        const auto wait_duration = heartbeat_interval - since_last_heartbeat;
        this_thread::sleep_for(wait_duration);
    }

    listen_thread.join();
}

void Central::listen() {
    const std::size_t buffer_size = 512;
    uint8_t buffer[buffer_size];

    while (running.load(memory_order_consume)) {
        std::size_t bytes_read;
        sockaddr_storage addr;

        {
            shared_lock<shared_mutex> lock(connection_lock);
            addr = connection.receive(buffer, buffer_size, bytes_read);
        }

        try {
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
            logError() << "Failed to parse message from " << connection.address_to_str(addr) << ": " << e.what();
        } catch (...) {
            logError() << "Unknown error occurred while parsing message from " << connection.address_to_str(addr);
        }
    }
}

void Central::run_sending_alerts() {
    Serializer serializer;

    while (running.load(memory_order_consume)) {
        auto alerts = alert_aggregator.extractAlerts();
        if (!alerts.empty()) {
            serializer.clear();
            for_each(alerts.begin(), alerts.end(), bind(&AbstractBlock::serialize, _1, serializer));

            uint16_t buffer_size;
            const uint8_t *buffer = serializer.get_buffer(buffer_size);

            shared_lock<shared_mutex> lock(connection_lock);
            connection.send_msg(buffer, buffer_size, get_fire_dept_address());
        }

        this_thread::sleep_for(get_alert_interval());
    }
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
            logWarn() << "Central::handle_block: invalid block type: " << block->type;
    }
}

void Central::send_heartbeat() {
    Serializer serializer;
    CentralServerHeartbeat(get_id()).serialize(serializer);

    uint16_t buffer_size;
    const uint8_t* buffer = serializer.get_buffer(buffer_size);

    shared_lock<shared_mutex> lock(connection_lock);
    connection.send_msg(buffer, buffer_size, get_fire_dept_address());
}

void Central::init_socket() {
    unique_lock<shared_mutex> lock(connection_lock);
    init_socket_impl();
}

void Central::init_socket_impl() {
    try {
        connection.open_socket();
        connection.bind_port(config.port);
    } catch (const exception &e) {
        logError() << "Exception occurred when initializing socket: " << e.what();
        throw;
    }
}

void Central::apply_port_change() {
    unique_lock<shared_mutex> lock(connection_lock);

    connection.close_socket();
    init_socket_impl();
}

std::chrono::milliseconds Central::get_heartbeat_interval() {
    shared_lock<shared_mutex> lock(config_lock);
    return chrono::milliseconds(config.heartbeet_interval_ms);
}

std::chrono::milliseconds Central::get_alert_interval() {
    shared_lock<shared_mutex> lock(config_lock);
    return chrono::milliseconds(config.alert_interval_ms);
}

int32_t Central::get_id() {
    shared_lock<shared_mutex> lock(config_lock);
    return config.id;
}

sockaddr_storage Central::get_fire_dept_address() {
    shared_lock<shared_mutex> lock(config_lock);
    return UdpConnection::get_address(config.fire_dept_ip, config.fire_dept_port);
}

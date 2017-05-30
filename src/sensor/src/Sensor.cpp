#include <Sensor.h>
#include <Deserializer.h>
#include <blocks/BlockReader.h>
#include <blocks/CntSensorConfigBlock.h>
#include <blocks/SensorMeasurementBlock.h>
#include <Logger.h>
#include <blocks/RequestConfigBlock.h>
#include <chrono>
#include <thread>
#include <blocks/SensorCommonBlock.h>

Sensor::Sensor(Serializer serializer) :
        serializer(serializer),
        config_reader("/home/igor/tin/src/core/resources/sensor-config.cfg") {

    config.sensor_port = config_reader.read_integer(config.SENSOR_PORT_PATH);
    config.id = config_reader.read_integer(config.SENSOR_ID_PATH);
    config.type = static_cast<SensorType>(config_reader.read_integer(config.SENSOR_TYPE_PATH));
    config.longitude = config_reader.read_integer(config.SENSOR_LONGITUDE);
    config.latitude = config_reader.read_integer(config.SENSOR_LATITUDE);
    config.cc_port = config_reader.read_integer(config.DEFAULT_CC_PORT);
    config.cc_addr = config_reader.read_string(config.DEFAULT_CC_IP);

    con_send.open_socket();
    con_recv.open_socket();
    init_recv_connection();
}

void Sensor::init_recv_connection() {
    con_recv.bind_port(config.sensor_port);
}

Sensor::~Sensor() {
    con_recv.close_socket();
    con_send.close_socket();
}

void Sensor::send_request_msg() {
    serializer.clear();
    RequestConfigBlock requestConfigBlock(config.sensor_port, config.id);
    requestConfigBlock.serialize(serializer);

    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);

    try {
        log() << "Sending: " << requestConfigBlock.toString();
        auto addr = UdpConnection::get_address(config.cc_addr, config.cc_port);
        con_send.send_msg(buffer, size, addr);
    } catch(const std::runtime_error& e) {
        logError() << e.what();
    }
}

bool Sensor::receive_cc_config_msg() {
    size_t bytes_read;

    try {
        con_recv.receive(buf, BUF_SIZE, bytes_read);
    } catch (const std::runtime_error& e) {
        logError() << e.what();
        return false;
    }

    logDebug() << "Received buffer size " << bytes_read;

    BlockReader reader(buf, bytes_read);

    for(auto& block : reader.blocks) {
        if(block->type == BlockType::cnt_sensor_config) {
            auto configBlock = reinterpret_cast<CntSensorConfigBlock*>(block.get());

            // TODO change add ip etc centrals jak sie przeniesie do configa
            reload_config(configBlock->port_id);

            central_ips.clear();
            for(auto& cnt_ip : configBlock->central_ips) {
                central_ips.push_back(cnt_ip);
            }

            log() << "Message: " << configBlock->toString();
        }
    }
    return true;
}

// TODO change
void Sensor::reload_config(in_port_t port) {
    config.sensor_port = port;
}

void Sensor::close_connection() {
    con_send.close_socket();
    con_recv.close_socket();
}

void Sensor::unset_connection_timeout() {
    con_recv.unset_connection_timeout();
}

void Sensor::set_connection_timeout(long int sec, long int microsec) {
    con_recv.set_connection_timeout(sec, microsec);
}

void Sensor::send_measurement(std::string central_ip, in_port_t port) {
    // TODO add locks on shared data

    serializer.clear();
    // TODO make the value being sent random
    SensorCommonBlock common_block(static_cast<uint64_t>(time(nullptr)), 55, 67, true);
    common_block.serialize(serializer);
    SensorMeasurementBlock measurement_block(BlockType::temp_read, 59.8);
    measurement_block.serialize(serializer);

    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);

    try {
        log() << "Sending: " << common_block.toString();
        log() << "Sending: " << measurement_block.toString();
        auto addr = UdpConnection::get_address(central_ip, port);
        con_send.send_msg(buffer, size, addr);
    } catch(const std::runtime_error& e) {
        logError() << e.what();
    }
}

void Sensor::broadcast_centrals()
{
    std::string delimiter(":");
    for (auto central : central_ips) {
        std::string ip(central.substr(0, central.find(delimiter)));
        std::string port(central.substr(ip.size() + 1, central.size() - ip.size()));

        send_measurement(ip, static_cast<uint16_t >(atoi(port.c_str())));
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

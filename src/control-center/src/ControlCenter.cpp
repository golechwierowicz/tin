#include <ControlCenter.h>
#include <Deserializer.h>
#include <iostream>
#include <blocks/CntSensorConfigBlock.h>
#include <Logger.h>
#include <blocks/BlockReader.h>
#include <blocks/RequestConfigBlock.h>

ControlCenter::ControlCenter(Serializer serializer, const std::string& filepath) : serializer(serializer) {
    cfg = new ConfigReader(filepath);
    port = cfg->read_integer(CC_PORT_PATH);
    ip = cfg->read_string(CC_IP_PATH);
    connection.open_socket();
    con_send.open_socket();
    init_connection();
}

ControlCenter::~ControlCenter() {
    delete cfg;
}

void ControlCenter::init_connection() {
    try {
        connection.bind_port(ControlCenter::port);
    } catch (const std::exception& e) {
        logError() << e.what();
        exit(1);
    }
}

std::vector<std::string> ControlCenter::get_central_ips() { 
    std::vector<std::string> ips; 
    std::vector<std::string> ports;

    int num_of_centrals = cfg->read_integer(CC_CENTRALS_SIZE);
    ips = cfg->read_string_arr(CC_CENTRALS_IPS, num_of_centrals);
    ports = cfg->read_string_arr(CC_CENTRALS_PORTS, num_of_centrals);

    std::vector<std::string> result;

    for(int i = 0; i < num_of_centrals; i++) 
        result.push_back(ips[i] + ":" + ports[i]);
    
    return result;
}

void ControlCenter::recv_sensor_request_msg() {
    try {
        size_t bytesRead;
        auto addr = connection.receive(buf, BUF_SIZE, bytesRead);
        BlockReader reader(buf, bytesRead);
        for (auto& block : reader.blocks) {
            if (block->type == BlockType::request_config) {
                auto requestConfigBlock = reinterpret_cast<RequestConfigBlock*>(block.get());
                UdpConnection::set_address_port(addr, requestConfigBlock->get_port());
                log() << UdpConnection::address_to_str(addr);
                log() << "Received: " << requestConfigBlock->toString();
                send_sensor_config_block(addr);
                update_sensor_list(requestConfigBlock->get_id(), addr);
            } else {
                logWarn() << "Received unexpected block: " << block->toString();
            }
        }
    } catch (const std::runtime_error& e) {
        logError() << e.what();
        return;
    }
}

void ControlCenter::send_sensor_config_block(const sockaddr_storage &addr) {
    serializer.clear();
    CntSensorConfigBlock sensorConfigBlock(get_central_ips(), port, ip);
    sensorConfigBlock.serialize(serializer);

    uint16_t bufSize;
    uint8_t* buf = serializer.get_buffer(bufSize);
    connection.send_msg(buf, bufSize, addr);
}                                                                                                                                                                                                                                                                                                                                                                                       

void ControlCenter::close_connection() {
    connection.close_socket();
    con_send.close_socket();
}

void ControlCenter::update_sensor_list(uint32_t sensor_id, sockaddr_storage addr) {
    auto sensors_iterator = sensors.find(sensor_id);
    if (sensors_iterator == sensors.end()) {
        sensors.insert(std::pair<uint32_t, sockaddr_storage>(sensor_id, addr));
        log() << "Control Center has registered a new sensor with id: " + std::to_string(sensor_id);
    }
}

void ControlCenter::broadcast_sensors() {
    for (auto sensor : sensors) {
        send_sensor_config_block(sensor.second);
    }
}

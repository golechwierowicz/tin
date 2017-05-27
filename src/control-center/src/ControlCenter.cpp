#include <ControlCenter.h>
#include <Deserializer.h>
#include <iostream>
#include <blocks/CntSensorConfigBlock.h>
#include <Logger.h>
#include <blocks/BlockReader.h>
#include <blocks/RequestConfigBlock.h>

ControlCenter::ControlCenter(Serializer serializer) : serializer(serializer) {
    connection.open_socket();
    con_send.open_socket();
    init_connection();
}

ControlCenter::~ControlCenter() {}

void ControlCenter::init_connection() {
    try {
        connection.bind_port(ControlCenter::port);
    } catch (const std::exception& e) {
        logError() << e.what();
        exit(1);
    }
}

std::vector<std::string> ControlCenter::get_central_ips() { // another dummy method, this should be hardcoded or read from config
    std::vector<std::string> result; 
    result.push_back("192.168.1.2");
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
                UdpConnection::setAddrPort(addr, requestConfigBlock->getPort());
                log() << UdpConnection::addressStr(addr);

                log() << "Received: " << requestConfigBlock->toString();

                serializer.clear();
                CntSensorConfigBlock sensorConfigBlock(get_central_ips(), port, ip);
                sensorConfigBlock.serialize(serializer);

                uint16_t bufSize;
                uint8_t* buf = serializer.get_buffer(bufSize);
                connection.send_msg(buf, bufSize, addr);
                //update_sensor_list(addr);
            } else {
                log() << "Received: " << block->toString();
            }
        }
    } catch (const std::runtime_error& e) {
        logError() << e.what();
        return;
    }
}

void ControlCenter::close_connection() {
    connection.close_socket();
    con_send.close_socket();
}

void ControlCenter::update_sensor_list(uint32_t sensor_id, sockaddr_storage addr) {
    auto sensors_iterator = sensors.find(sensor_id);
    if (sensors_iterator == sensors.end()) {
        sensors.insert(std::pair<uint32_t, sockaddr_storage>(sensor_id, addr));
        log() << "Control Center has registered a new sensor";
    }
}
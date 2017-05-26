#include <ControlCenter.h>
#include <Deserializer.h>
#include <CommonBlock.h>
#include <iostream>
#include <blocks/CntSensorConfigBlock.h>
#include <Logger.h>
#include <blocks/BlockReader.h>
#include <blocks/RequestConfigBlock.h>
#include "AddressInfo.h"

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
        for (AbstractBlock* block : reader.blocks) {
            if (block->type == bt_request_config) {
                auto requestConfigBlock = (RequestConfigBlock*) block;
                log() << "Received: " << requestConfigBlock->toString();
                addr.setPort(requestConfigBlock->getPort());
                serializer.clear();
                CntSensorConfigBlock block(get_central_ips(), port, ip);
                block.serialize(serializer);
                uint16_t bufSize;
                uint8_t* buf = serializer.get_buffer(bufSize);
                connection.send_msg(buf, bufSize, addr);
                //update_sensor_list(addr);
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

void ControlCenter::update_sensor_list(uint32_t sensor_id, sockaddr_4or6 addr) {
    auto sensors_iterator = sensors.find(sensor_id);
    if (sensors_iterator == sensors.end()) {
        sensors.insert(std::pair<uint32_t, sockaddr_4or6>(sensor_id, addr));
        log() << "Control Center has registered a new sensor";
    }
}
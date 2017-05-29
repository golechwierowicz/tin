//
// Created by igor on 25.05.17.
//

#include "blocks/RequestConfigBlock.h"

using namespace std;

void RequestConfigBlock::serialize(Serializer &serializer) {
    serializer
        .begin_block(type)
        .write<uint16_t>(port)
        .write<uint32_t>(sensor_id)
        .end_block();
}

std::unique_ptr<RequestConfigBlock> RequestConfigBlock::deserialize(Deserializer &deserializer) {
    in_port_t port;
    uint32_t sensor_id;

    deserializer
        .read<uint16_t>(port)
        .read<uint32_t>(sensor_id);

    return make_unique<RequestConfigBlock>(port, sensor_id);
}

std::string RequestConfigBlock::toString() {
    std::stringstream ss;
    ss << "RequestConfigBlock [id:" << sensor_id << " port: " << port << "]";
    return ss.str();
}

in_port_t RequestConfigBlock::get_port() const {
    return port;
}

uint32_t RequestConfigBlock::get_id() const {
    return sensor_id;
}

//
// Created by igor on 25.05.17.
//

#include "blocks/RequestConfigBlock.h"

void RequestConfigBlock::serialize(Serializer &serializer) {
    serializer
        .begin_block(type)
        .write<uint16_t>(port)
        .end_block();
}

void RequestConfigBlock::deserialize(Deserializer &deserializer) {
    deserializer
        .read<uint16_t>(port);
}

std::string RequestConfigBlock::toString() {
    std::stringstream ss;
    ss << "RequestConfigBlock [" << port << "]";
    return ss.str();
}

in_port_t RequestConfigBlock::getPort() const {
    return port;
}

void RequestConfigBlock::setPort(in_port_t port) {
    RequestConfigBlock::port = port;
}

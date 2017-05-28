//
// Created by igor on 25.05.17.
//

#include "blocks/RequestConfigBlock.h"

using namespace std;

void RequestConfigBlock::serialize(Serializer &serializer) {
    serializer
        .begin_block(type)
        .write<uint16_t>(port)
        .end_block();
}

std::unique_ptr<RequestConfigBlock> RequestConfigBlock::deserialize(Deserializer &deserializer) {
    in_port_t port;

    deserializer
        .read<uint16_t>(port);

    return make_unique<RequestConfigBlock>(port);
}

std::string RequestConfigBlock::toString() {
    std::stringstream ss;
    ss << "RequestConfigBlock [" << port << "]";
    return ss.str();
}

in_port_t RequestConfigBlock::getPort() const {
    return port;
}

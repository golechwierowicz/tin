//
// Created by igor on 25.05.17.
//

#include "blocks/RequestConfigBlock.h"

void RequestConfigBlock::serialize(Serializer &serializer) {
    serializer
        .begin_block(type)
        .end_block();
}

void RequestConfigBlock::deserialize(Deserializer &deserializer) {
    return;
}

std::string RequestConfigBlock::toString() {
    return "RequestConfigBlock []";
}

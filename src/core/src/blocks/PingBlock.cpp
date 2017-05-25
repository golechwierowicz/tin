//
// Created by igor on 25.05.17.
//

#include "blocks/PingBlock.h"

void PingBlock::serialize(Serializer &serializer) {
    serializer
            .begin_block(type)
            .end_block();
}

void PingBlock::deserialize(Deserializer &deserializer) {
    return;
}

std::string PingBlock::toString() {
    return "PingBlock []";
}

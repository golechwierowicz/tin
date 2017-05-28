//
// Created by igor on 25.05.17.
//

#include "blocks/PingBlock.h"

using namespace std;

void PingBlock::serialize(Serializer &serializer) {
    serializer
            .begin_block(type)
            .end_block();
}

std::unique_ptr<PingBlock> PingBlock::deserialize(Deserializer &deserializer) {
    return make_unique<PingBlock>();
}

std::string PingBlock::toString() {
    return "PingBlock []";
}

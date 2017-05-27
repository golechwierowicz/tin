//
// Created by igor on 25.05.17.
//

#ifndef TIN_PINGBLOCK_H
#define TIN_PINGBLOCK_H


#include "AbstractBlock.h"
#include <memory>

struct PingBlock : public AbstractBlock {
    PingBlock() : AbstractBlock(BlockType::ping) {}

    void serialize(Serializer& serializer);
    static std::unique_ptr<PingBlock> deserialize(Deserializer& deserializer);
    std::string toString();
};


#endif //TIN_PINGBLOCK_H

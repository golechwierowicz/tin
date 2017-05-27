//
// Created by igor on 25.05.17.
//

#ifndef TIN_PINGBLOCK_H
#define TIN_PINGBLOCK_H


#include "AbstractBlock.h"

class PingBlock : public AbstractBlock {
public:
    PingBlock() : AbstractBlock(BlockType::ping) {}

    void serialize(Serializer& serializer);
    void deserialize(Deserializer& deserializer);
    std::string toString();
};


#endif //TIN_PINGBLOCK_H

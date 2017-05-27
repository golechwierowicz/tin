#ifndef TIN_ABSTRACTBLOCK_H
#define TIN_ABSTRACTBLOCK_H

#include <Serializer.h>
#include <Deserializer.h>
#include "BlockType.h"

class AbstractBlock {
public:
    AbstractBlock(BlockType type) : type(type) {}

    const BlockType type;

    virtual void serialize(Serializer& serializer) = 0;

    virtual std::string toString() = 0;
};


#endif //TIN_ABSTRACTBLOCK_H

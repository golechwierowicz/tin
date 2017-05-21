#ifndef TIN_ABSTRACTBLOCK_H
#define TIN_ABSTRACTBLOCK_H

#include <Serializer.h>
#include <Deserializer.h>

enum BlockType { debug };

class AbstractBlock {
public:
    BlockType type;

    virtual void serialize(Serializer& serializer) = 0;
    virtual void deserialize(Deserializer& deserializer) = 0;

    virtual std::string toString() = 0;
};


#endif //TIN_ABSTRACTBLOCK_H

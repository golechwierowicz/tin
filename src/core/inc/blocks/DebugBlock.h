#ifndef TIN_DEBUGBLOCK_H
#define TIN_DEBUGBLOCK_H


#include "AbstractBlock.h"

class DebugBlock : public AbstractBlock {
    BlockType type = BlockType::debug;

    uint8_t u8_value;
    int64_t i64_value;

    std::string str_value;

    void serialize(Serializer& serializer);
    void deserialize(Deserializer& deserializer);
    std::string toString();
};


#endif //TIN_DEBUGBLOCK_H

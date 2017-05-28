#ifndef TIN_DEBUGBLOCK_H
#define TIN_DEBUGBLOCK_H


#include "AbstractBlock.h"
#include <memory>

struct DebugBlock : public AbstractBlock {
    DebugBlock(uint8_t u8_value = 0, int64_t i64_value = 0, const std::string &str_value = "");

    uint8_t u8_value;
    int64_t i64_value;

    std::string str_value;

    void serialize(Serializer& serializer);
    static std::unique_ptr<DebugBlock> deserialize(Deserializer& deserializer);
    std::string toString();
};


#endif //TIN_DEBUGBLOCK_H

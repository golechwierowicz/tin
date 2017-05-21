#include <sstream>
#include "blocks/DebugBlock.h"

void DebugBlock::serialize(Serializer& serializer) {
    serializer
            .begin_block(type)
            .write(u8_value)
            .write(i64_value)
            .write(str_value)
            .end_block();
}

void DebugBlock::deserialize(Deserializer& deserializer) {
    deserializer
            .read(u8_value)
            .read(i64_value)
            .read(str_value);
}

std::string DebugBlock::toString() {
    std::stringstream ss;
    ss << "DebugBlock ["
       << "u8: " << u8_value << ", "
       << "i64: " << i64_value << ", "
       << "str: " << str_value
       << "]";

    return ss.str();
}

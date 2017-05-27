#include <sstream>
#include "blocks/DebugBlock.h"

void DebugBlock::serialize(Serializer& serializer) {
    serializer
            .begin_block(type)
            .write<uint8_t>(u8_value)
            .write<int64_t>(i64_value)
            .write<const std::string&>(str_value)
            .end_block();
}

void DebugBlock::deserialize(Deserializer& deserializer) {
    deserializer
            .read<uint8_t>(u8_value)
            .read<int64_t>(i64_value)
            .read<std::string>(str_value);
}

std::string DebugBlock::toString() {
    std::stringstream ss;
    ss << "DebugBlock [" << std::hex
       << "u8: 0x" << (unsigned) u8_value << ", "
       << "i64: 0x" << i64_value << ", "
       << "str: " << str_value
       << "]";

    return ss.str();
}

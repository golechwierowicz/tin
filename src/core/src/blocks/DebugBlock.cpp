#include <sstream>
#include "blocks/DebugBlock.h"

using namespace std;

DebugBlock::DebugBlock(uint8_t u8_value, int64_t i64_value, const string &str_value)
        : AbstractBlock(BlockType::debug),
          u8_value(u8_value),
          i64_value(i64_value),
          str_value(str_value) {}

void DebugBlock::serialize(Serializer& serializer) {
    serializer
            .begin_block(type)
            .write<uint8_t>(u8_value)
            .write<int64_t>(i64_value)
            .write<const std::string&>(str_value)
            .end_block();
}

std::unique_ptr<DebugBlock> DebugBlock::deserialize(Deserializer& deserializer) {
    uint8_t u8_value;
    int64_t i64_value;
    std::string str_value;

    deserializer
            .read<uint8_t>(u8_value)
            .read<int64_t>(i64_value)
            .read<std::string>(str_value);

    return make_unique<DebugBlock>(u8_value, i64_value, str_value);
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

#include "blocks/SensorCommonBlock.h"
#include <sstream>

void SensorCommonBlock::serialize(Serializer &serializer) {
    serializer.begin_block(type)
            .write<uint64_t>(timestamp)
            .write<uint16_t>(latitude)
            .write<uint16_t>(longitude)
            .write<bool>(is_alive)
            .end_block();
}

void SensorCommonBlock::deserialize(Deserializer &deserializer) {
    deserializer
            .read<uint64_t>(timestamp)
            .read<uint16_t>(latitude)
            .read<uint16_t>(longitude)
            .read<bool>(is_alive);
}

std::string SensorCommonBlock::toString() {
    std::stringstream str;
    str << "SensorCommonBlock ["
        << "timestamp=" << timestamp << "; "
        << "latitude=" << latitude << "; "
        << "longitude=" << longitude << "; "
        << "is_alive=" << (is_alive ? "true" : "false")
        << "]";
    return str.str();
}

SensorCommonBlock::SensorCommonBlock(uint64_t timestamp, uint16_t latitude, uint16_t longitude, bool is_alive)
        : AbstractBlock(BlockType::sensor_common),
          timestamp(timestamp),
          latitude(latitude),
          longitude(longitude),
          is_alive(is_alive) {}

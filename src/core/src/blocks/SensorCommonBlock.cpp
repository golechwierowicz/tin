#include "blocks/SensorCommonBlock.h"
#include <sstream>

using namespace std;

void SensorCommonBlock::serialize(Serializer &serializer) {
    serializer.begin_block(type)
            .write<uint64_t>(timestamp)
            .write<uint16_t>(latitude)
            .write<uint16_t>(longitude)
            .write<bool>(is_alive)
            .end_block();
}

std::unique_ptr<SensorCommonBlock> SensorCommonBlock::deserialize(Deserializer &deserializer) {
    uint64_t timestamp;
    uint16_t latitude;
    uint16_t longitude;
    bool is_alive;

    deserializer
            .read<uint64_t>(timestamp)
            .read<uint16_t>(latitude)
            .read<uint16_t>(longitude)
            .read<bool>(is_alive);

    return make_unique<SensorCommonBlock>(timestamp, latitude, longitude, is_alive);
}

std::string SensorCommonBlock::toString() {
    stringstream str;
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

uint64_t SensorCommonBlock::get_timestamp() const {
    return timestamp;
}

uint16_t SensorCommonBlock::get_latitude() const {
    return latitude;
}

uint16_t SensorCommonBlock::get_longitude() const {
    return longitude;
}

bool SensorCommonBlock::get_is_alive() const {
    return is_alive;
}

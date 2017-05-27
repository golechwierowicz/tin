#include "blocks/CentralServerFireAlert.h"
#include <sstream>

void CentralServerFireAlert::serialize(Serializer &serializer) {
    serializer.begin_block(type)
            .write<uint64_t>(timestamp)
            .write<uint16_t>(latitude)
            .write<uint16_t>(longitude)
            .write<uint32_t>(alerts_count)
            .end_block();
}

void CentralServerFireAlert::deserialize(Deserializer &deserializer) {
    deserializer
            .read<uint64_t>(timestamp)
            .read<uint16_t>(latitude)
            .read<uint16_t>(longitude)
            .read<uint32_t>(alerts_count);
}

std::string CentralServerFireAlert::toString() {
    std::stringstream str;
    str << "CentralServerFireAlert ["
        << "timestamp=" << timestamp << "; "
        << "latitude=" << latitude << "; "
        << "longitude=" << longitude << "; "
        << "alerts_count=" << alerts_count
        << "]";
    return str.str();
}

CentralServerFireAlert::CentralServerFireAlert(
        uint64_t timestamp, uint16_t latitude, uint16_t longitude, uint32_t alerts_count)
        : AbstractBlock(BlockType::central_server_fire_alert),
          timestamp(timestamp),
          latitude(latitude),
          longitude(longitude),
          alerts_count(alerts_count) {}

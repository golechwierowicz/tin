#include "blocks/CentralServerHeartbeat.h"
#include <sstream>

CentralServerHeartbeat::CentralServerHeartbeat(int32_t id)
        : AbstractBlock(BlockType::central_server_heartbeat),
          id(id) {}

void CentralServerHeartbeat::serialize(Serializer &serializer) {
    serializer.begin_block(type)
            .write<int32_t>(id)
            .end_block();
}

void CentralServerHeartbeat::deserialize(Deserializer &deserializer) {
    deserializer
            .read<int32_t>(id);
}

std::string CentralServerHeartbeat::toString() {
    std::stringstream str;
    str << "CentralServerHeartbeat ["
        << "id=" << id
        << "]";
    return str.str();
}

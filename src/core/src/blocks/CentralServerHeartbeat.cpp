#include "blocks/CentralServerHeartbeat.h"
#include <sstream>

using namespace std;

CentralServerHeartbeat::CentralServerHeartbeat(int32_t id)
        : AbstractBlock(BlockType::central_server_heartbeat),
          id(id) {}

void CentralServerHeartbeat::serialize(Serializer &serializer) {
    serializer.begin_block(type)
            .write<int32_t>(id)
            .end_block();
}

std::unique_ptr<CentralServerHeartbeat> CentralServerHeartbeat::deserialize(Deserializer &deserializer) {
    int32_t id;

    deserializer
            .read<int32_t>(id);

    return make_unique<CentralServerHeartbeat>(id);
}

std::string CentralServerHeartbeat::toString() {
    stringstream str;
    str << "CentralServerHeartbeat ["
        << "id=" << id
        << "]";
    return str.str();
}

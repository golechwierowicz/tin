#include <sstream>
#include <iterator>
#include "blocks/CntSensorConfigBlock.h"

using namespace std;

CntSensorConfigBlock::CntSensorConfigBlock(std::vector<std::string> central_ips, in_port_t port_id, std::string cnt_ip)
        : AbstractBlock(BlockType::cnt_sensor_config),
        central_ips(central_ips),
        port_id(port_id),
        cnt_ip(cnt_ip) {}

void CntSensorConfigBlock::serialize(Serializer &serializer) {
    serializer.begin_block(type)
            .write<uint16_t>(port_id)
            .write<const std::string&>(cnt_ip)
            .write<uint64_t>(central_ips.size());
    for(auto central_ip : central_ips) {
        serializer.write<const std::string&>(central_ip);
    }
    serializer.end_block();
}

std::unique_ptr<CntSensorConfigBlock> CntSensorConfigBlock::deserialize(Deserializer &deserializer) {
    std::vector<std::string> central_ips;
    in_port_t port_id;
    std::string cnt_ip;

    size_t size;
    deserializer
            .read<uint16_t>(port_id)
            .read<std::string>(cnt_ip)
            .read<uint64_t>(size);

    std::string s;
    for(size_t i = 0; i < size; i++) {
        deserializer.read<std::string>(s);
        central_ips.push_back(s);
    }

    return make_unique<CntSensorConfigBlock>(move(central_ips), port_id, move(cnt_ip));
}

std::string CntSensorConfigBlock::toString() {
    std::stringstream ss;
    ss << "CntSensorConfigBlock ["
       << "port_id: " << port_id << ", "
       << "cnt_ip: " << cnt_ip << ", "
       << "central_ips: {";

    std::copy(central_ips.begin(),
              central_ips.end() - 1,
              std::ostream_iterator<std::string>(ss, ", "));
    ss << central_ips.back();

    ss << "}]";

    return ss.str();
}

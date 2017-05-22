#include <sstream>
#include <iterator>
#include "blocks/CntSensorConfigBlock.h"

void CntSensorConfigBlock::serialize(Serializer &serializer) {
    serializer.begin_block(type)
            .write(port_id)
            .write(cnt_ip)
            .write(central_ips.size());
    for(auto central_ip : central_ips) {
        serializer.write(central_ip);
    }
    serializer.end_block();
}

void CntSensorConfigBlock::deserialize(Deserializer &deserializer) {
    size_t size;
    deserializer
            .read(port_id)
            .read(cnt_ip)
            .read(size);
    central_ips.clear();
    for(size_t i = 0; i < size; i++) {
        std::string s;
        deserializer.read(s);
        central_ips.push_back(s);
    }
}

std::string CntSensorConfigBlock::toString() {
    std::stringstream ss;
    ss << "CntSensorConfigBlocvk ["
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

CntSensorConfigBlock::CntSensorConfigBlock(std::vector<std::string> central_ips,
                                           in_port_t port_id,
                                           std::string cnt_ip) :
        central_ips(central_ips),
        port_id(port_id),
        cnt_ip(cnt_ip) {}

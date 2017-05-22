//
// Created by piotr on 5/22/17.
//

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

}

std::string CntSensorConfigBlock::toString() {
    return std::__cxx11::string();
}

CntSensorConfigBlock::CntSensorConfigBlock(std::vector<std::string> central_ips,
                                           in_port_t port_id,
                                           std::string cnt_ip) :
        central_ips(central_ips),
        port_id(port_id),
        cnt_ip(cnt_ip) {}

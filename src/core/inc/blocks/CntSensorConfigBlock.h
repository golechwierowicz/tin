#ifndef TIN_CNTSENSORCONFIGBLOCK_H
#define TIN_CNTSENSORCONFIGBLOCK_H


#include <vector>
#include <netinet/in.h>
#include "AbstractBlock.h"
#include <memory>

struct CntSensorConfigBlock : public AbstractBlock {
    std::vector<std::string> central_ips;
    in_port_t port_id;
    std::string cnt_ip;

    CntSensorConfigBlock(std::vector<std::string> central_ips,
                         in_port_t port_id,
                         std::string cnt_ip);

    void serialize(Serializer& serializer);
    static std::unique_ptr<CntSensorConfigBlock> deserialize(Deserializer& deserializer);
    std::string toString();
};


#endif //TIN_CNTSENSORCONFIGBLOCK_H

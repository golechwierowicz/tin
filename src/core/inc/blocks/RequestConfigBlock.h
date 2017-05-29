//
// Created by igor on 25.05.17.
//

#ifndef TIN_REQUESTCONFIGBLOCK_H
#define TIN_REQUESTCONFIGBLOCK_H


#include "AbstractBlock.h"
#include <sstream>
#include <netinet/in.h>
#include <memory>

class RequestConfigBlock : public AbstractBlock {
    in_port_t port;
    uint32_t sensor_id;
    
public:
    RequestConfigBlock(in_port_t port, uint32_t sensor_id) : 
            AbstractBlock(BlockType::request_config), port(port), sensor_id(sensor_id) {}

    void serialize(Serializer& serializer);
    static std::unique_ptr<RequestConfigBlock> deserialize(Deserializer& deserializer);
    std::string toString();

    in_port_t get_port() const;
    uint32_t get_id() const;
};


#endif //TIN_REQUESTCONFIGBLOCK_H

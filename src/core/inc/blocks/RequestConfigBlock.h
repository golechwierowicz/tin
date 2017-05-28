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

public:
    RequestConfigBlock(in_port_t port) : AbstractBlock(BlockType::request_config), port(port) {}

    void serialize(Serializer& serializer);
    static std::unique_ptr<RequestConfigBlock> deserialize(Deserializer& deserializer);
    std::string toString();

    in_port_t getPort() const;
};


#endif //TIN_REQUESTCONFIGBLOCK_H

//
// Created by igor on 25.05.17.
//

#ifndef TIN_REQUESTCONFIGBLOCK_H
#define TIN_REQUESTCONFIGBLOCK_H


#include "AbstractBlock.h"
#include <sstream>
#include <netinet/in.h>

class RequestConfigBlock : public AbstractBlock {
private:
    in_port_t port;
public:
    in_port_t getPort() const;

    void setPort(in_port_t port);

public:
    RequestConfigBlock(in_port_t port = 0) : AbstractBlock(bt_request_config), port(port) {}

    void serialize(Serializer& serializer);
    void deserialize(Deserializer& deserializer);
    std::string toString();
};


#endif //TIN_REQUESTCONFIGBLOCK_H

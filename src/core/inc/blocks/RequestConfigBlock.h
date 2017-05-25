//
// Created by igor on 25.05.17.
//

#ifndef TIN_REQUESTCONFIGBLOCK_H
#define TIN_REQUESTCONFIGBLOCK_H


#include "AbstractBlock.h"

class RequestConfigBlock : public AbstractBlock {
public:
    RequestConfigBlock() : AbstractBlock(bt_request_config) {}

    void serialize(Serializer& serializer);
    void deserialize(Deserializer& deserializer);
    std::string toString();
};


#endif //TIN_REQUESTCONFIGBLOCK_H

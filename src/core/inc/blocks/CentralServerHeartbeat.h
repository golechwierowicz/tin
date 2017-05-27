#pragma once

#include "AbstractBlock.h"

class CentralServerHeartbeat : public AbstractBlock {
    int32_t id;

public:
    CentralServerHeartbeat(int32_t id = 0);

    void serialize(Serializer &serializer) override;

    void deserialize(Deserializer &deserializer) override;

    std::string toString() override;
};
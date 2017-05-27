#pragma once

#include "AbstractBlock.h"
#include <memory>

class CentralServerHeartbeat : public AbstractBlock {
    int32_t id;

public:
    CentralServerHeartbeat(int32_t id);

    void serialize(Serializer &serializer) override;

    static std::unique_ptr<CentralServerHeartbeat> deserialize(Deserializer &deserializer);

    std::string toString() override;
};
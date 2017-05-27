#pragma once

#include "AbstractBlock.h"

class SensorCommonBlock : public AbstractBlock {
    uint64_t timestamp;
    uint16_t latitude;
    uint16_t longitude;
    bool is_alive;

public:
    SensorCommonBlock(uint64_t timestamp = 0, uint16_t latitude = 0, uint16_t longitude = 0, bool is_alive = false);

    void serialize(Serializer &serializer) override;

    void deserialize(Deserializer &deserializer) override;

    std::string toString() override;
};
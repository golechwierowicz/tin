#pragma once

#include "AbstractBlock.h"

class CentralServerFireAlert : public AbstractBlock {
    uint64_t timestamp;
    uint16_t latitude;
    uint16_t longitude;
    uint32_t alerts_count;

public:
    CentralServerFireAlert(
            uint64_t timestamp = 0,
            uint16_t latitude = 0,
            uint16_t longitude = 0,
            uint32_t alerts_count = 0
    );

    void serialize(Serializer &serializer) override;

    void deserialize(Deserializer &deserializer) override;

    std::string toString() override;

};
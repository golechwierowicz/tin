#pragma once

#include "AbstractBlock.h"
#include <memory>

class CentralServerFireAlert : public AbstractBlock {
    uint64_t timestamp;
    uint16_t latitude;
    uint16_t longitude;
    uint32_t alerts_count;

public:
    CentralServerFireAlert(
            uint64_t timestamp,
            uint16_t latitude,
            uint16_t longitude,
            uint32_t alerts_count
    );

    void serialize(Serializer &serializer);

    static std::unique_ptr<CentralServerFireAlert> deserialize(Deserializer &deserializer);

    std::string toString() override;

    uint64_t get_timestamp() const;

    uint16_t get_latitude() const;

    uint16_t get_longitude() const;

    uint32_t get_alerts_count() const;
};
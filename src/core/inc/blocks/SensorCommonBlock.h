#pragma once

#include "AbstractBlock.h"
#include <memory>

class SensorCommonBlock : public AbstractBlock {
    uint64_t timestamp;
    uint16_t latitude;
    uint16_t longitude;
    bool is_alive;

public:
    SensorCommonBlock(uint64_t timestamp, uint16_t latitude, uint16_t longitude, bool is_alive);

    void serialize(Serializer &serializer) override;

    static std::unique_ptr<SensorCommonBlock> deserialize(Deserializer &deserializer);

    std::string toString() override;

    uint64_t get_timestamp() const;

    uint16_t get_latitude() const;

    uint16_t get_longitude() const;

    bool get_is_alive() const;
};
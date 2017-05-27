#pragma once

#include <cstdint>

struct CentralConfig {
    uint16_t port;

    uint32_t heartbeet_interval_ms;
    uint32_t alert_interval_ms;
};
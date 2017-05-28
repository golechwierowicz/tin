#pragma once

#include <cstdint>

struct CentralConfig {
    int32_t id;

    uint16_t port;

    uint32_t heartbeet_interval_ms;
    uint32_t alert_interval_ms;

    std::string fire_dept_ip;
    uint16_t fire_dept_port;
};
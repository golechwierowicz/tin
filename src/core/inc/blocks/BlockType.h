#pragma once

#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <string>

enum class BlockType : uint32_t {
    debug = 0,
    sensor_common = 1,
    smoke_read = 2,
    ir_read = 3,
    temp_read = 4,
    central_server_fire_alert = 5,
    central_server_heartbeat = 6,
    cnt_sensor_config = 7,
    request_config = 8,
    ping = 9
};

std::ostream &operator<<(std::ostream &o, const BlockType &block_type);
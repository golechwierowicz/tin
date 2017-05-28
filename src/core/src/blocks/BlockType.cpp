#include "blocks/BlockType.h"

namespace {
    struct BlockTypeHash {
        std::size_t operator()(BlockType bt) const noexcept {
            return static_cast<uint32_t>(bt);
        }
    };
}

std::ostream &operator<<(std::ostream &o, const BlockType &block_type) {
    static std::unordered_map<BlockType, std::string, BlockTypeHash> block_type_to_string = {
            {BlockType::debug, "BlockType::debug"},
            {BlockType::sensor_common, "BlockType::sensor_common"},
            {BlockType::smoke_read, "BlockType::smoke_read"},
            {BlockType::ir_read, "BlockType::ir_read"},
            {BlockType::temp_read, "BlockType::temp_read"},
            {BlockType::central_server_fire_alert, "BlockType::central_server_fire_alert"},
            {BlockType::central_server_heartbeat, "BlockType::central_server_heartbeat"},
            {BlockType::cnt_sensor_config, "BlockType::cnt_sensor_config"},
            {BlockType::request_config, "BlockType::request_config"},
            {BlockType::ping, "BlockType::ping"}
    };

    return o << block_type_to_string[block_type];
}

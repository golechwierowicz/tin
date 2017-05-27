#include <blocks/DebugBlock.h>
#include <Logger.h>
#include <blocks/CntSensorConfigBlock.h>
#include <blocks/RequestConfigBlock.h>
#include <blocks/SensorMeasurementBlock.h>
#include "blocks/CentralServerFireAlert.h"
#include "blocks/CentralServerHeartbeat.h"
#include "blocks/BlockReader.h"
#include <unordered_map>
#include <functional>
#include <blocks/SensorCommonBlock.h>
#include <blocks/PingBlock.h>

using namespace std;

namespace {
    using placeholders::_1;

    struct BlockTypeHash {
        size_t operator()(BlockType block_type) const {
            return static_cast<uint32_t>(block_type);
        }
    };

    unordered_map<
            BlockType,
            function<unique_ptr<AbstractBlock>(Deserializer&)>,
            BlockTypeHash
    >  deserializers = {
            {BlockType::debug, &DebugBlock::deserialize},
            {BlockType::sensor_common, &SensorCommonBlock::deserialize},
            {BlockType::smoke_read, bind(&SensorMeasurementBlock::deserialize, _1, BlockType::smoke_read)},
            {BlockType::ir_read, bind(&SensorMeasurementBlock::deserialize, _1, BlockType::ir_read)},
            {BlockType::temp_read, bind(&SensorMeasurementBlock::deserialize, _1, BlockType::temp_read)},
            {BlockType::central_server_fire_alert, &CentralServerFireAlert::deserialize},
            {BlockType::central_server_heartbeat, &CentralServerHeartbeat::deserialize},
            {BlockType::cnt_sensor_config, &CntSensorConfigBlock::deserialize},
            {BlockType::request_config, &RequestConfigBlock::deserialize},
            {BlockType::ping, &PingBlock::deserialize}
    };
}

BlockReader::BlockReader(const uint8_t *message_buffer, size_t message_size) :
        message_buffer(message_buffer),
        message_size(message_size) {

    Deserializer deserializer(message_buffer, (uint32_t) message_size);
    while (deserializer.next_block()) {
        auto make_block = deserializers.find(deserializer.get_block_type());

        if (make_block != deserializers.end()) {
            blocks.push_back(make_block->second(deserializer));
        } else {
            logWarn() << "BlockReader: Skipping unknown block type";
        }
    }
}

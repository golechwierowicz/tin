#include <blocks/DebugBlock.h>
#include <Logger.h>
#include <blocks/CntSensorConfigBlock.h>
#include <blocks/RequestConfigBlock.h>
#include <blocks/SensorMeasurementBlock.h>
#include "blocks/CentralServerFireAlert.h"
#include "blocks/CentralServerHeartbeat.h"
#include "blocks/BlockReader.h"

using namespace std;

BlockReader::BlockReader(const uint8_t *message_buffer, size_t message_size) :
        message_buffer(message_buffer),
        message_size(message_size) {

    Deserializer deserializer(message_buffer, (uint32_t) message_size);
    while (deserializer.next_block()) {
        switch (deserializer.get_block_type()) {
            case BlockType::debug: {
                auto block = make_unique<DebugBlock>();
                block->deserialize(deserializer);
                blocks.push_back(move(block));
                break;
            }

            case BlockType::smoke_read:
            case BlockType::ir_read:
            case BlockType::temp_read: {
                auto block = make_unique<SensorMeasurementBlock>(deserializer.get_block_type());
                block->deserialize(deserializer);
                blocks.push_back(move(block));
                break;
            }

            case BlockType::cnt_sensor_config: {
                auto block = make_unique<CntSensorConfigBlock>();
                block->deserialize(deserializer);
                blocks.push_back(move(block));
                break;
            }

            case BlockType::central_server_fire_alert: {
                auto block = make_unique<CentralServerFireAlert>();
                block->deserialize(deserializer);
                blocks.push_back(move(block));
                break;
            }

            case BlockType::central_server_heartbeat: {
                auto block = make_unique<CentralServerHeartbeat>();
                block->deserialize(deserializer);
                blocks.push_back(move(block));
                break;
            }

            case BlockType::request_config: {
                auto block = make_unique<RequestConfigBlock>();
                block->deserialize(deserializer);
                blocks.push_back(move(block));
                break;
            }

            default: {
                logWarn() << "BlockReader: Skipping unknown block type";
                break;
            }
        }
    }
}

#include <blocks/DebugBlock.h>
#include <Logger.h>
#include <blocks/CntSensorConfigBlock.h>
#include <blocks/RequestConfigBlock.h>
#include <blocks/SensorMeasurementBlock.h>
#include "blocks/BlockReader.h"

BlockReader::BlockReader(uint8_t *message_buffer, size_t message_size) :
        message_buffer(message_buffer),
        message_size(message_size) {
    Deserializer deserializer(message_buffer, (uint32_t) message_size);
    while (deserializer.next_block()) {
        switch (deserializer.get_block_type()) {
            case bt_debug: {
                DebugBlock* block = new DebugBlock;
                block->deserialize(deserializer);
                blocks.push_back((AbstractBlock*)block);
            } break;
            case bt_smoke_read:
            case bt_IR_read:
            case bt_temp_read: {
                SensorMeasurementBlock* block = new SensorMeasurementBlock(
                        static_cast<BlockType>(deserializer.get_block_type())
                );
                block->deserialize(deserializer);
                blocks.push_back((AbstractBlock*)block);
            } break;
            case bt_cnt_sensor_config: {
                CntSensorConfigBlock* block = new CntSensorConfigBlock;
                block->deserialize(deserializer);
                blocks.push_back((AbstractBlock*) block);
            } break;
            case bt_request_config: {
                RequestConfigBlock* block = new RequestConfigBlock;
                block->deserialize(deserializer);
                blocks.push_back((AbstractBlock*)block);
            } break;
            default: {
                logWarn() << "Skipping unknown block type";
            } break;
        }
    }
}

BlockReader::~BlockReader() {
    for (auto it = blocks.begin() ; it != blocks.end(); ++it) {
        delete (*it);
    }
}

#ifndef TIN_ABSTRACTBLOCK_H
#define TIN_ABSTRACTBLOCK_H

#include <Serializer.h>
#include <Deserializer.h>

enum BlockType {
    bt_debug = 0,
    bt_smoke_read = 2,
    bt_ir_read = 3,
    bt_temp_read = 4,
    bt_cnt_sensor_config = 7,
    bt_request_config = 8,
    bt_ping = 9
};

class AbstractBlock {
public:
    AbstractBlock(BlockType type) : type(type) {}

    BlockType type;

    virtual void serialize(Serializer& serializer) = 0;
    virtual void deserialize(Deserializer& deserializer) = 0;

    virtual std::string toString() = 0;
};


#endif //TIN_ABSTRACTBLOCK_H

//
// Created by igor on 26.05.17.
//

#include <sstream>
#include "blocks/SensorMeasurementBlock.h"

using namespace std;

void SensorMeasurementBlock::serialize(Serializer& serializer) {
    serializer
        .begin_block(type)
        .write<double>(read_value)
        .end_block();
}

std::unique_ptr<SensorMeasurementBlock> SensorMeasurementBlock::deserialize(
        Deserializer& deserializer, BlockType block_type) {

    double read_value;
    deserializer
        .read<double>(read_value);

    return make_unique<SensorMeasurementBlock>(block_type, read_value);
}

std::string SensorMeasurementBlock::toString() {
    std::stringstream ss;
    ss << "SensorMeasurementBlock [" << read_value << "]";
    return ss.str();
}

double SensorMeasurementBlock::getReadValue() const {
    return read_value;
}

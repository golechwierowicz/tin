//
// Created by igor on 26.05.17.
//

#include <sstream>
#include "blocks/SensorMeasurementBlock.h"

void SensorMeasurementBlock::serialize(Serializer& serializer) {
    serializer
        .begin_block(type)
        .write<double>(readValue)
        .end_block();
}

void SensorMeasurementBlock::deserialize(Deserializer& deserializer) {
    deserializer
        .read<double>(readValue);
}

std::string SensorMeasurementBlock::toString() {
    std::stringstream ss;
    ss << "SensorMeasurementBlock [" << readValue << "]";
    return ss.str();
}

double SensorMeasurementBlock::getReadValue() const {
    return readValue;
}

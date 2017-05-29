//
// Created by igor on 26.05.17.
//

#ifndef TIN_SENSORMEASUREMENTBLOCK_H
#define TIN_SENSORMEASUREMENTBLOCK_H


#include "AbstractBlock.h"
#include <memory>

class SensorMeasurementBlock : public AbstractBlock{
    double read_value;

public:
    SensorMeasurementBlock(BlockType block_type, double read_value)
            : AbstractBlock(block_type),
              read_value(read_value) {}

    void serialize(Serializer&);
    static std::unique_ptr<SensorMeasurementBlock> deserialize(Deserializer& deserializer, BlockType block_type);
    std::string toString();

    double getReadValue() const;
};


#endif //TIN_SENSORMEASUREMENTBLOCK_H

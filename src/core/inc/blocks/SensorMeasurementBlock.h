//
// Created by igor on 26.05.17.
//

#ifndef TIN_SENSORMEASUREMENTBLOCK_H
#define TIN_SENSORMEASUREMENTBLOCK_H


#include "AbstractBlock.h"

class SensorMeasurementBlock : public AbstractBlock{
public:
    SensorMeasurementBlock(enum BlockType blockType) : AbstractBlock(blockType) {}
    SensorMeasurementBlock(enum BlockType blockType, double readValue) : AbstractBlock(blockType){}
    void serialize(Serializer&);
    void deserialize(Deserializer&);
    std::string SensorMeasurementBlock::toString();
    double getReadValue() const;
private:
    double readValue;
};


#endif //TIN_SENSORMEASUREMENTBLOCK_H

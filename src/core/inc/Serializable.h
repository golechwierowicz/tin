#ifndef TIN_SERIALIZABLE_H
#define TIN_SERIALIZABLE_H

#include "Serializer.h"
#include "Deserializer.h"

class Serializable {
public:
    virtual void serialize(const Serializer& serializer) = 0;
    virtual void deserialize(const Deserializer& deserializer) = 0;
};

#endif //TIN_SERIALIZABLE_H

#include <iostream>
#include "Serializer.h"

Serializer& Serializer::begin_block(int32_t type) {
    assert(!in_block);
    in_block = true;
    last_block = buffer_position;
    write(type);
    write((int32_t) 0);
}

Serializer& Serializer::write(void* value, size_t size)  {
    assert(in_block);
    memcpy(buffer + buffer_position, value, size);
    buffer_position += size;
    return *this;
}

Serializer& Serializer::end_block() {
    assert(in_block);
    in_block = false;
    int32_t size = buffer_position - last_block;
    memcpy(buffer + last_block + sizeof(int32_t), &size, sizeof(int32_t));
}

Serializer& Serializer::write(int8_t value) {
    write(&value, sizeof(value));
}

Serializer& Serializer::write(int16_t value) {
    write(&value, sizeof(value));
}

Serializer& Serializer::write(int32_t value) {
    write(&value, sizeof(value));
}

Serializer& Serializer::write(int64_t value) {
    write(&value, sizeof(value));
}
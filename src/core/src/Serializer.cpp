#include <iostream>
#include "Serializer.h"

Serializer& Serializer::begin_block(int32_t type) {
    assert(!in_block);
    in_block = true;
    last_block = buffer_position;
    write(type);
    write((int32_t) 0);
    return *this;
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
    return *this;
}

Serializer& Serializer::write(int8_t value) {
    return write(&value, sizeof(value));
}

Serializer& Serializer::write(int16_t value) {
    return write((uint16_t) value);
}

Serializer& Serializer::write(int32_t value) {
    return write((uint32_t) value);
}

Serializer& Serializer::write(int64_t value) {
    return write((uint64_t) value);
}

Serializer& Serializer::write(uint8_t value) {
    return write(&value, sizeof(value));
}

Serializer& Serializer::write(uint16_t value) {
    uint16_t to_be_written = htobe16(value);
    return write(&to_be_written, sizeof(to_be_written));
}

Serializer& Serializer::write(uint32_t value) {
    uint32_t to_be_written = htobe32(value);
    return write(&to_be_written, sizeof(to_be_written));
}

Serializer& Serializer::write(uint64_t value) {
    uint64_t to_be_written = htobe64(value);
    return write(&to_be_written, sizeof(to_be_written));
}
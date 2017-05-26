#include <cstring>
#include <string>
#include "Serializer.h"

Serializer& Serializer::begin_block(uint32_t type) {
    assert(!in_block);
    in_block = true;
    write<uint32_t>(type);
    write<int32_t>(0);
    return *this;
}

Serializer& Serializer::write(const void* value, size_t size) {
    assert(in_block);
    if (buffer_position + size > BUFFER_SIZE) {
        throw std::runtime_error("No space left in buffer");
    } else {
        memcpy(buffer + buffer_position, value, size);
        buffer_position += size;
        return *this;
    }
}

Serializer& Serializer::end_block() {
    assert(in_block);
    uint32_t size = htobe32((uint32_t) (buffer_position - last_block));
    memcpy(buffer + last_block + sizeof(uint32_t), &size, sizeof(uint32_t));
    last_block = buffer_position;
    in_block = false;
    return *this;
}

void Serializer::clear() {
    buffer_position = 0;
    last_block = 0;
    bool in_block = false;
    std::fill_n(buffer, BUFFER_SIZE, 0);
}

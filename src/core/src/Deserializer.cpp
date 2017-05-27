#include <cstring>
#include <stdexcept>
#include "Deserializer.h"

bool Deserializer::next_block() {
    buffer_position = last_block + block_size;
    last_block = buffer_position;
    if (buffer_position > buffer_size) {
        throw std::runtime_error("Block size exceeded buffer size");
    } else if(buffer_position == buffer_size) {
        return false;
    }

    block_size = 8; // allow read of type and size
    read<uint32_t>(block_type);
    read<uint32_t>(block_size);

    return true;
}

Deserializer& Deserializer::read(void* out_buffer, size_t size) {
    if(buffer_position + size > buffer_size) {
        throw std::runtime_error("Cannot read more bytes than the buffer contains");
    } else if(buffer_position + size > last_block + block_size) {
        throw std::runtime_error("Cannot read more bytes than the given block size");
    }
    memcpy(out_buffer, buffer + buffer_position, size);
    buffer_position += size;
    return *this;
}

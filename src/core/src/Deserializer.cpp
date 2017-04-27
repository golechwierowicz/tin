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
    read(block_type);
    read(block_size);

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

Deserializer& Deserializer::read(int8_t& value) {
    return read(&value, sizeof(value));
}

Deserializer& Deserializer::read(int16_t& value) {
    return read((uint16_t&) value);
}

Deserializer& Deserializer::read(int32_t& value) {
    return read((uint32_t&) value);
}

Deserializer& Deserializer::read(int64_t& value) {
    return read((uint64_t&) value);
}

Deserializer& Deserializer::read(uint8_t& value) {
    return read(&value, sizeof(value));
}

Deserializer& Deserializer::read(uint16_t& value) {
    uint16_t stored;
    read(&stored, sizeof(stored));
    value = be16toh(stored);
    return *this;
}

Deserializer& Deserializer::read(uint32_t& value) {
    uint32_t stored;
    read(&stored, sizeof(stored));
    value = be32toh(stored);
    return *this;
}

Deserializer& Deserializer::read(uint64_t& value) {
    uint64_t stored;
    read(&stored, sizeof(stored));
    value = be64toh(stored);
    return *this;
}

Deserializer& Deserializer::read(std::string& value) {
    uint8_t size;
    read(size);
    char buffer[size];
    read(buffer, size);
    value = std::string(buffer, size);
    return *this;
}

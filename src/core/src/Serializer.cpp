#include <cstring>
#include <string>
#include <stdexcept>
#include "Serializer.h"

Serializer& Serializer::begin_block(uint32_t type) {
    assert(!in_block);
    in_block = true;
    write(type);
    write((int32_t) 0);
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
    memcpy(buffer + last_block + sizeof(int32_t), &size, sizeof(int32_t));
    last_block = buffer_position;
    in_block = false;
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

Serializer& Serializer::write(const std::string& s) {
    if (s.size() > 0xFF) {
        throw std::runtime_error("Only strings of size smaller than 256 are allowed");
    } else {
        write((uint8_t) s.size());
        write(s.c_str(), s.size());
        return *this;
    }
}

#ifndef TIN_SERIALIZER_H
#define TIN_SERIALIZER_H

#include <cstdint>
#include <string>
#include <assert.h>

class Serializer {
private:
    static const int BUFFER_SIZE = 512;

    uint8_t buffer[BUFFER_SIZE];
    uint16_t buffer_position = 0;
    uint16_t last_block = 0;
    bool in_block = false;

    Serializer& write(const void* value, size_t size);

public:
    Serializer& begin_block(int32_t type);
    Serializer& end_block();

    Serializer& write(int8_t value);
    Serializer& write(int16_t value);
    Serializer& write(int32_t value);
    Serializer& write(int64_t value);
    Serializer& write(uint8_t value);
    Serializer& write(uint16_t value);
    Serializer& write(uint32_t value);
    Serializer& write(uint64_t value);
    Serializer& write(const std::string& s);

    uint8_t* get_buffer(uint16_t& size) {
        size = last_block;
        return buffer;
    }
};

#endif //TIN_SERIALIZER_H
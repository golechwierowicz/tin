#ifndef TIN_SERIALIZER_H
#define TIN_SERIALIZER_H

#include <cstdint>
#include <cstring>
#include <assert.h>

namespace {
    static const int BUFF_SIZE = 512;
}

class Serializer {
private:
    uint8_t buffer[BUFF_SIZE];
    int16_t buffer_position = 0;
    int16_t last_block = 0;
    bool in_block = false;

    Serializer& write(void* value, size_t size);

public:
    Serializer& begin_block(int32_t type);
    Serializer& end_block();

    Serializer& write(int8_t value);
    Serializer& write(int16_t value);
    Serializer& write(int32_t value);
    Serializer& write(int64_t value);

    uint8_t* get_buffer() { return buffer; }
};

#endif //TIN_SERIALIZER_H
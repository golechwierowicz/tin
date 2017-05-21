#ifndef TIN_DESERIALIZER_H
#define TIN_DESERIALIZER_H

#include <cstdint>
#include <memory>

class Deserializer {
private:
    uint8_t* buffer;
    uint32_t buffer_size;
    uint32_t buffer_position;
    uint32_t last_block = 0;
    uint32_t block_size = 0;
    uint32_t block_type = 0;

    Deserializer& read(void* out_buffer, size_t size);

public:
    Deserializer(uint8_t* buffer, uint32_t buffer_size)
            : buffer(buffer), buffer_size(buffer_size) {
        next_block();
    }

    bool next_block();
    uint32_t get_block_type() { return block_type; }

    Deserializer& read(int8_t& value);
    Deserializer& read(int16_t& value);
    Deserializer& read(int32_t& value);
    Deserializer& read(int64_t& value);
    Deserializer& read(uint8_t& value);
    Deserializer& read(uint16_t& value);
    Deserializer& read(uint32_t& value);
    Deserializer& read(uint64_t& value);
    Deserializer& read(std::string& value);
};


#endif //TIN_DESERIALIZER_H

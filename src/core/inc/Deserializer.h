#ifndef TIN_DESERIALIZER_H
#define TIN_DESERIALIZER_H

#include <cstdint>
#include <memory>

class Deserializer {
    uint8_t* buffer;
    uint32_t buffer_size;
    uint32_t buffer_position = 0;
    uint32_t last_block = 0;
    uint32_t block_size = 0;

public:
    Deserializer(uint8_t* buffer, uint32_t buffer_size)
            : buffer(buffer), buffer_size(buffer_size) {
        advance();
    }

    /**
     * Advance to the next block
     * @return false if the last block was already read
     */
    bool advance();

    void read(int8_t& value);
    void read(int16_t& value);
    void read(int32_t& value);
    void read(int64_t& value);
    void read(uint8_t& value);
    void read(uint16_t& value);
    void read(uint32_t& value);
    void read(uint64_t& value);
    void read(std::string& value);
};


#endif //TIN_DESERIALIZER_H

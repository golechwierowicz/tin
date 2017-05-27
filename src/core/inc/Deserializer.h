#ifndef TIN_DESERIALIZER_H
#define TIN_DESERIALIZER_H

#include <cstdint>
#include <memory>
#include <limits>

class Deserializer {
public:
    template<typename T>
    struct TypeTag {
        using type = T;
    };

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
        if(buffer_size < 8) {
            throw std::runtime_error("Buffer to small to contain a block");
        }
    }

    bool next_block();
    uint32_t get_block_type() { return block_type; }

    template <typename T>
    Deserializer& read(typename TypeTag<T>::type &value) {
        typename std::conditional<std::is_same<T, T>::value, void, char>::type sfinae[0];
    }
};

template <>
inline Deserializer& Deserializer::read<uint8_t>(Deserializer::TypeTag<uint8_t>::type& value) {
    return read(&value, sizeof(value));
}

template <>
inline Deserializer& Deserializer::read<uint16_t>(Deserializer::TypeTag<uint16_t>::type& value) {
    uint16_t stored;
    read(&stored, sizeof(stored));
    value = be16toh(stored);
    return *this;
}

template <>
inline Deserializer& Deserializer::read<uint32_t>(Deserializer::TypeTag<uint32_t>::type& value) {
    uint32_t stored;
    read(&stored, sizeof(stored));
    value = be32toh(stored);
    return *this;
}

template <>
inline Deserializer& Deserializer::read<uint64_t>(Deserializer::TypeTag<uint64_t>::type& value) {
    uint64_t stored;
    read(&stored, sizeof(stored));
    value = be64toh(stored);
    return *this;
}

template <>
inline Deserializer& Deserializer::read<int8_t>(Deserializer::TypeTag<int8_t>::type& value) {
    return read(&value, sizeof(value));
}

template <>
inline Deserializer& Deserializer::read<int16_t>(Deserializer::TypeTag<int16_t>::type& value) {
    uint16_t stored;
    read<uint16_t>(stored);
    memcpy(&value, &stored, sizeof(value));
    return *this;
}

template <>
inline Deserializer& Deserializer::read<int32_t>(Deserializer::TypeTag<int32_t>::type& value) {
    uint32_t stored;
    read<uint32_t>(stored);
    memcpy(&value, &stored, sizeof(value));
    return *this;
}

template <>
inline Deserializer& Deserializer::read<int64_t>(Deserializer::TypeTag<int64_t>::type& value) {
    uint64_t stored;
    read<uint64_t>(stored);
    memcpy(&value, &stored, sizeof(value));
    return *this;
}

template <>
inline Deserializer& Deserializer::read<double>(Deserializer::TypeTag<double>::type &value) {
    static_assert(std::numeric_limits<double>::is_iec559, "non-standard float implementation");
    static_assert(sizeof(double) == sizeof(uint64_t), "unsupported double size");

    uint64_t storage;
    read<uint64_t>(storage);
    memcpy(&value, &storage, sizeof(value));

    return *this;
}

template <>
inline Deserializer& Deserializer::read<std::string>(Deserializer::TypeTag<std::string>::type& value) {
    uint8_t size;
    read<uint8_t>(size);
    char buffer[size];
    read(buffer, size);
    value = std::string(buffer, size);
    return *this;
}



#endif //TIN_DESERIALIZER_H

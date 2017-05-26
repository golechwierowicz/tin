#ifndef TIN_SERIALIZER_H
#define TIN_SERIALIZER_H

#include <cstdint>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <type_traits>
#include <stdexcept>
#include <cstring>
#include <limits>

class Serializer {
public:
    static const int BUFFER_SIZE = 512;

    template <typename T>
    struct TypeTag {
        using type = T;
    };

private:
    uint8_t buffer[BUFFER_SIZE];
    uint16_t buffer_position = 0;
    uint16_t last_block = 0;
    bool in_block = false;

    Serializer& write(const void* value, size_t size);

public:
    Serializer& begin_block(uint32_t type);
    Serializer& end_block();

    template <typename T>
    Serializer& write(typename TypeTag<T>::type value) {
        typename std::conditional<std::is_same<T, T>::value, void, char>::type sfinae[0];
    }

    uint8_t* get_buffer(uint16_t& size) {
        size = last_block;
        return buffer;
    }

    void clear();
};

template <>
inline Serializer& Serializer::write<uint8_t>(Serializer::TypeTag<uint8_t>::type value) {
    return write(&value, sizeof(value));
}

template <>
inline Serializer& Serializer::write<uint16_t>(Serializer::TypeTag<uint16_t>::type value) {
    uint16_t to_be_written = htobe16(value);
    return write(&to_be_written, sizeof(to_be_written));
}

template <>
inline Serializer& Serializer::write<uint32_t>(Serializer::TypeTag<uint32_t>::type value) {
    uint32_t to_be_written = htobe32(value);
    return write(&to_be_written, sizeof(to_be_written));
}

template <>
inline Serializer& Serializer::write<uint64_t>(Serializer::TypeTag<uint64_t>::type value) {
    uint64_t to_be_written = htobe64(value);
    return write(&to_be_written, sizeof(to_be_written));
}

template <>
inline Serializer& Serializer::write<int8_t>(Serializer::TypeTag<int8_t>::type value) {
    return write(&value, sizeof(value));
}

template <>
inline Serializer& Serializer::write<int16_t>(Serializer::TypeTag<int16_t>::type value) {
    uint16_t storage;
    memcpy(&storage, &value, sizeof(value));
    return write<uint16_t>(storage);
}

template <>
inline Serializer& Serializer::write<int32_t>(Serializer::TypeTag<int32_t>::type value) {
    uint32_t storage;
    memcpy(&storage, &value, sizeof(value));
    return write<uint32_t>(storage);
}

template <>
inline Serializer& Serializer::write<double>(Serializer::TypeTag<double>::type value) {
    static_assert(std::numeric_limits<double>::is_iec559);
    static_assert(sizeof(double) == sizeof(uint64_t));

    uint64_t storage;
    memcpy(&storage, &value, sizeof(double));
    return write<uint64_t>(storage);
}

template <>
inline Serializer& Serializer::write<int64_t>(Serializer::TypeTag<int64_t>::type value) {
    uint64_t storage;
    memcpy(&storage, &value, sizeof(value));
    return write<uint64_t>(storage);
}

template <>
inline Serializer& Serializer::write<const std::string&>(Serializer::TypeTag<const std::string&>::type s) {
    if (s.size() > 0xFF) {
        throw std::runtime_error("Only strings of size smaller than 256 are allowed");
    } else {
        write<uint8_t>(static_cast<uint8_t>(s.size()));
        write(s.c_str(), s.size());
        return *this;
    }
}

#endif //TIN_SERIALIZER_H

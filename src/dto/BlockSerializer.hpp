namespace {
    static const int BUFF_SIZE = 512;
}

class Serializer {
private:
    char buff_char[BUFF_SIZE]; 
    int_fast16_t buff_ptr = 0;

public:
    Serializer& write(int32_t value) {
        memcopy(buff_char + buff_ptr, sizeof(value), &value);
        buff_ptr += sizeof(value);
        return *this;
    }

    Serializer& write(int64_t value) {
        memcopy(buff_char + buff_ptr, sizeof(value), &value);
        buff_ptr += sizeof(value);
        return *this;
    }

    // get

    int8_t* get_buffer() {
        return buff_char;
    }
}

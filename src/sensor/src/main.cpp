#include <iostream>
#include <Serializer.h>

int main() {
    Serializer serializer;
    serializer.begin_block(1)
            .write((int32_t) 2)
            .write((int8_t) 123)
            .end_block();
    return 0;
}
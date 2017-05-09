#include <iostream>
#include <Serializer.h>
#include <Sensor.h>

int main() {
    Serializer serializer;
//    serializer.begin_block(1)
//            .write((int32_t) 2)
//            .write((int8_t) 123)
//            .end_block();

    Sensor sensor(serializer);
    sensor.send_test_msg();
    //sensor.receive_test_msg();
    return 0;
}
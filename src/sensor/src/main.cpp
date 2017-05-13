#include <iostream>
#include <Serializer.h>
#include <Sensor.h>

int main() {
    Serializer serializer;
    Sensor sensor(serializer);
    sensor.send_test_msg();
    return 0;
}
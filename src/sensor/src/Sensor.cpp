#include <Sensor.h>
#include <iostream>

Sensor::Sensor(Serializer serializer){
    // dummy method, will need to implement reading conf form file
    config = init_config();
    this->serializer = serializer;
    con = new Connection();
    con->create_socket();
}

Sensor::~Sensor() {
    con->close_socket();
    delete con;
}
// to be changed, needs to read conf from file/info sent by CC/any kind of init conf
Config* Sensor::init_config() {
    return new Config(5, 5, 5, 4048);
}

void Sensor::send_test_msg() {
    serializer.begin_block(1)
            .write(std::string("serialized string"))
            .write(std::string("\nsent by Igor\n"))
            .end_block();

    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);
    con->send_data(buffer, size, config->getCc_port());
}

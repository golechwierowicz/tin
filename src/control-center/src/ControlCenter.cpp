#include <ControlCenter.h>
#include <Deserializer.h>
#include <Serializer.h>
#include <iostream>

//
// Created by igor on 09.05.17.
//

ControlCenter::ControlCenter(Serializer serializer) {
    _serializer = serializer;
    connection = new Connection();
    con_send = new Connection();
    connection->create_socket();
    con_send->create_socket();
    init_connection();
    //init_sensor_connection("127.0.0.1", 4049);
}

ControlCenter::~ControlCenter() {}

void ControlCenter::init_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &my_name.sin_addr);
    my_name.sin_port = htons(ControlCenter::port);

    if (bind(connection->_socket, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("controlcenter, init_connection: binding datagram socket");
    }
}

void ControlCenter::init_sensor_connection(std::string sens_addr, in_port_t sens_port) {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, sens_addr.c_str(), &my_name.sin_addr);
    my_name.sin_port = htons(sens_port);

    if (bind(con_send->_socket, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("controlcenter, init_connection: binding datagram socket");
    }

}

void ControlCenter::send_test_sensor_msg() {
    _serializer.begin_block(1)
            .write(std::string("serialized string"))
            .write(std::string("\nsent by Igor\n"))
            .end_block();

    uint16_t size;
    uint8_t* buffer = _serializer.get_buffer(size);
    connection->send_data(buffer, size, 4049, "127.0.0.1"); // TODO: fix this, this should come from some map
}

void ControlCenter::recv_test_sensor_msg() {
    uint8_t buf[512];
    struct sockaddr_in cli_name;
    socklen_t addrlen;

    addrlen = sizeof(cli_name);
    if (recvfrom(connection->_socket, buf, 512, 0, (struct sockaddr*)&cli_name, &addrlen) == -1) {
        perror("receiving datagram packet");
    }

    send_test_sensor_msg();

    Deserializer d(buf, sizeof(buf));
    std::string string_value_1;
    std::string string_value_2;
    d.read(string_value_1);
    d.read(string_value_2);
    std::cout << string_value_1 << " " << string_value_2;
}

void ControlCenter::close_connection() {
    connection->close_socket();
}

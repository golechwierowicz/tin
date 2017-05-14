#include <ControlCenter.h>
#include <Deserializer.h>
#include <iostream>

//
// Created by igor on 09.05.17.
//

ControlCenter::ControlCenter() {
    connection = new Connection();
    connection->create_socket();
    init_connection();
}

ControlCenter::~ControlCenter() {}

void ControlCenter::init_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &my_name.sin_addr);
    my_name.sin_port = htons(ControlCenter::port);

    if (bind(connection->_socket, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("binding datagram socket");
    }
}

void ControlCenter::send_test_sensor_msg() {
}
void ControlCenter::recv_test_sensor_msg() {
    uint8_t buf[512];
    struct sockaddr_in cli_name;
    socklen_t addrlen;

    addrlen = sizeof(cli_name);
    if (recvfrom(connection->_socket, buf, 512, 0, (struct sockaddr*)&cli_name, &addrlen) == -1) {
        perror("receiving datagram packet");
    }

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

#include <ControlCenter.h>
#include <Deserializer.h>
#include <iostream>

//
// Created by igor on 09.05.17.
//

ControlCenter::ControlCenter() {

}

void ControlCenter::send_test_sensor_msg() {

}
void ControlCenter::recv_test_sensor_msg() {
    int sockd;
    struct sockaddr_in my_name, cli_name;
    uint8_t buf[512];
    socklen_t addrlen;

    /* create a socket */
    sockd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockd == -1) {
        perror("Socket creation error");
        exit(1);
    }
    /* server address */
    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &my_name.sin_addr);
    my_name.sin_port = htons(10000);

    if (bind(sockd, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("binding datagram socket");
    }
    addrlen = sizeof(cli_name);
    if (recvfrom(sockd, buf, 1024, 0, (struct sockaddr*)&cli_name, &addrlen) == -1) {
        perror("receiving datagram packet");
    }

    Deserializer d(buf, sizeof(buf));
    std::string string_value;
    d.read(string_value);
    std::cout << string_value;

    close(sockd);
    exit(0);
}

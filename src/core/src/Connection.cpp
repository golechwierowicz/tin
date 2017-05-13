//
// Created by igor on 13.05.17.
//

#include <Connection.h>
#include <iostream>

Connection::Connection() {}

Connection::~Connection() {}

int Connection::create_socket() {
    _socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (_socket == -1)
    {
        perror("Socket creation error");
        exit(1);
    }
}

int Connection::close_socket() {
    close(_socket);
}

void Connection::receive_init() {

}

void Connection::send_data(uint8_t* data, uint16_t size, in_port_t port) {
    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &srv_addr.sin_addr);
    srv_addr.sin_port = htons(port);
    if (sendto(_socket, data, size, 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        perror("error sending data to CC form sensor");
    }
}

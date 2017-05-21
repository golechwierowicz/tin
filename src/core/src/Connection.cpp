#include <Connection.h>
#include <iostream>
#include <unistd.h>

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

void Connection::send_data(uint8_t* data, uint16_t size, in_port_t port, std::string addr) {
    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    inet_aton(addr.c_str(), &srv_addr.sin_addr);
    srv_addr.sin_port = htons(port);
    if (sendto(_socket, data, size, 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        perror("error sending data.");
    }
}

//
// Created by igor on 13.05.17.
//

#ifndef TIN_CONNECTION_H
#define TIN_CONNECTION_H

#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <string>

class Connection {
public:
    Connection();
    ~Connection();
    int create_socket();
    int close_socket();
    void send_data(uint8_t* data, uint16_t size, in_port_t port, std::string addr);
    void receive_init();
    void receive_data();
//private:
    int _socket;
};


#endif //TIN_CONNECTION_H

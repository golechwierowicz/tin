#ifndef TIN_UDP_CONNECTION_H
#define TIN_UDP_CONNECTION_H

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include "errorhandling.h"

struct sockaddr_4or6 {
    bool ipv4;
    struct sockaddr_in addr4;
    struct sockaddr_in6 addr6;

    std::string toString();
    void setPort(in_port_t port) {
        addr4.sin_port = htons(port);
        addr6.sin6_port = htons(port);
    }
//    bool operator==(const sockaddr_4or6& o) {
//        return addr6.sin6_port == o.addr6.sin6_port && addr6.sin6_addr ? true : false;
//    }
};

class UdpConnection {
private:
    bool open = false;
public:
    /* TEMPORARILY PUBLIC! Remove all usages */
    int socket_fd;
    static std::string LOCALHOST;

    ~UdpConnection();

    void open_socket();
    void close_socket();

    void bind_port(uint16_t port);

    /* DEPRECATED */
    void send_data(uint8_t* data, uint16_t size, in_port_t port, std::string addr);

    static sockaddr_4or6 getAddress(std::string addr, in_port_t port);
    void send_msg(uint8_t* buffer, size_t len, sockaddr_4or6& address);

    sockaddr_4or6 receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);
};


#endif //TIN_UDP_CONNECTION_H

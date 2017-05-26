#ifndef TIN_UDP_CONNECTION_H
#define TIN_UDP_CONNECTION_H

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include "errorhandling.h"

class UdpConnection {
private:
    bool open = false;
    int socket_fd;
    
public:
    static const std::string LOCALHOST;

    ~UdpConnection();

    void open_socket();
    void close_socket();

    void bind_port(uint16_t port);
    void send_msg(uint8_t* buffer, size_t len, sockaddr_storage address);
    sockaddr_storage receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);


    static sockaddr_storage getAddress(const std::string& addr, in_port_t port);
    static std::string addressStr(sockaddr_storage& address);
    static void setAddrPort(sockaddr_storage* address, in_port_t port);
};


#endif //TIN_UDP_CONNECTION_H

#ifndef TIN_UDPSERVER_H
#define TIN_UDPSERVER_H

#include <cstdint>

class UdpServer {
private:
    int ipv4_socket_fd;
public:
    UdpServer(uint16_t port);
    ~UdpServer();
    void recieve(uint8_t* buffer, uint16_t& size);
};


#endif //TIN_UDPSERVER_H

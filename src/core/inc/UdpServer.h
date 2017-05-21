#ifndef TIN_UDPSERVER_H
#define TIN_UDPSERVER_H

#include <cstdint>
#include <cstdlib>

class UdpServer {
private:
    int ipv4_socket_fd;
public:
    UdpServer(uint16_t port);
    ~UdpServer();
    void receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);
};


#endif //TIN_UDPSERVER_H

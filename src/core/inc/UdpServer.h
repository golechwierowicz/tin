#ifndef TIN_UDPSERVER_H
#define TIN_UDPSERVER_H

#include <cstdint>
#include <cstdlib>

class UdpServer {
private:
    int socket_fd;

    void create_socket(uint16_t port);
public:
    UdpServer(uint16_t port);
    ~UdpServer();
    void receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);
};


#endif //TIN_UDPSERVER_H

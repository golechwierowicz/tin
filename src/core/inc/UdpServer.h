#ifndef TIN_UDPSERVER_H
#define TIN_UDPSERVER_H

#include <cstdint>

class UdpServer {
public:
    UdpServer();
    ~UdpServer();
    void recieve(uint8_t* buffer, uint16_t& size);
};


#endif //TIN_UDPSERVER_H

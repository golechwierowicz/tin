#ifndef TIN_UDPSERVER_H
#define TIN_UDPSERVER_H

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <cstring>

class UdpServer {
private:
    int socket_fd;
public:
    void open(uint16_t port);
    ~UdpServer();
    void receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);

    void throwErrno(const char* message) {
        std::stringstream ss;
        ss << "UdpServer: "
           << message
           << " ("
           << strerror(errno)
           << ")";
        throw std::runtime_error(ss.str());
    }
};


#endif //TIN_UDPSERVER_H

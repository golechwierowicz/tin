#ifndef TIN_UDP_CONNECTION_H
#define TIN_UDP_CONNECTION_H

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <cstring>

class UdpConnection {
private:
    int socket_fd;
public:
    UdpConnection();
    ~UdpConnection();

    void bind(uint16_t port);

    void send(uint8_t* buffer, size_t buffer_size, const std::string& addr, in_port_t port);
    void receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);

    void throwErrno(const char* message) {
        std::stringstream ss;
        ss << "UdpConnection: "
           << message
           << " ("
           << strerror(errno)
           << ")";
        throw std::runtime_error(ss.str());
    }
};


#endif //TIN_UDP_CONNECTION_H

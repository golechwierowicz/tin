#ifndef TIN_UDP_CONNECTION_H
#define TIN_UDP_CONNECTION_H

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <netinet/in.h>

class UdpConnection {
private:
    int socket_fd;
    bool open = false;
public:
    static constexpr const char* LOCALHOST = "127.0.0.1";

    ~UdpConnection();

    void open_socket();
    void close_socket();

    void bind_port(uint16_t port);

    void send(uint8_t* buffer, size_t buffer_size, const std::string& addr, in_port_t port);
    void receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);

    void raiseError(const char *message, const char *error = strerror(errno)) {
        std::stringstream ss;
        ss << "UdpConnection: "
           << message
           << " ("
           << error
           << ")";
        throw std::runtime_error(ss.str());
    }
};


#endif //TIN_UDP_CONNECTION_H

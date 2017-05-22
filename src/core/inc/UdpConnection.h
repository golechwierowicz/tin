#ifndef TIN_UDP_CONNECTION_H
#define TIN_UDP_CONNECTION_H

#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <netinet/in.h>

struct sockaddr_4or6 {
    bool ipv4;
    struct sockaddr_in addr4;
    struct sockaddr_in6 addr6;
};

class UdpConnection {
private:
    bool open = false;
public:
    /* TEMPORARILY PUBLIC! Remove all usages */
    int socket_fd;

    static constexpr const char* LOCALHOST = "127.0.0.1";

    ~UdpConnection();

    void open_socket();
    void close_socket();

    void bind_port(uint16_t port);

    /* DEPRECATED */
    void send_data(uint8_t* data, uint16_t size, in_port_t port, std::string addr);

    sockaddr_4or6 getAddress(std::string& addr, in_port_t port);
    void send_msg(uint8_t* buffer, size_t len, sockaddr_4or6& address);

    void receive(uint8_t* buffer, size_t buffer_size, size_t& data_length);

    void raiseError(const char *message, const char *error = strerror(errno)) {
        std::stringstream ss;
        ss << "UdpConnection: "
           << message
           << " (" << error << ")";
        throw std::runtime_error(ss.str());
    }
};


#endif //TIN_UDP_CONNECTION_H

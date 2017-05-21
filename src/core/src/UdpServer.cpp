#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include "UdpServer.h"
#include "logging.h"

void UdpServer::open(uint16_t port) {
    socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        std::stringstream ss;
        ss << "UdpServer: Socket creation failed ("
           << strerror(errno) << ")";
        throw std::runtime_error(ss.str());
    }

    struct sockaddr_in6 address;

    memset((char *)&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any; // all interfaces (0.0.0.0)
    address.sin6_port = htons(port);

    int bind_result = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));

    if (bind_result < 0) {
        std::stringstream ss;
        ss << "UdpServer: Socket bind failed ("
           << strerror(errno) << ")";
        throw std::runtime_error(ss.str());
    }

    log_debug("UdpServer: Opened");
}

UdpServer::~UdpServer() {
    close(socket_fd);
    log_debug("UdpServer: Disposed");
}

void UdpServer::receive(uint8_t* buffer, size_t buffer_size, size_t& data_length) {
    log_debug("UdpServer: Waiting for message");

    struct sockaddr_in remote_address;
    socklen_t addrlen = sizeof(remote_address);

    ssize_t value = recvfrom(
            socket_fd,
            buffer,
            buffer_size,
            0, // FLAGS
            (struct sockaddr *)&remote_address, &addrlen);

    if(value < 0) {
        std::stringstream ss;
        ss << "UdpServer: Message receive failed ("
           << strerror(errno) << ")";
        throw std::runtime_error(ss.str());
    } else {
        log_debug("UdpServer: Message received");
        data_length = (size_t) value;
    }
}

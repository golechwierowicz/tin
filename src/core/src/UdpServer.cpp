#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "UdpServer.h"
#include "logging.h"

UdpServer::UdpServer(uint16_t port) {
    create_socket(port);
    log_debug("UdpServer: Created");
}

void UdpServer::create_socket(uint16_t port) {
    socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        log_error("UdpServer: Socket creation failed");
        log_errno();
        exit(0);
    }

    struct sockaddr_in6 address;

    memset((char *)&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any; // all interfaces (0.0.0.0)
    address.sin6_port = htons(port);

    int bind_result = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));

    if (bind_result < 0) {
        log_error("UdpServer: Socket bind failed");
        log_errno();
        exit(0);
    }
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
        log_error("UdpServer: Message receive failed");
        log_errno();
        data_length = 0;
    } else {
        log_debug("UdpServer: Message received");
        data_length = (size_t) value;
    }
}
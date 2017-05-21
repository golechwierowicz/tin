#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "UdpServer.h"
#include "logging.h"

UdpServer::UdpServer(uint16_t port) {
    ipv4_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (ipv4_socket_fd < 0) {
        log_error("UdpServer: Socket creation failed");
        log_errno();
        exit(0);
    }

    struct sockaddr_in ipv4_address;

    memset((char *)&ipv4_address, 0, sizeof(ipv4_address));
    ipv4_address.sin_family = AF_INET;
    ipv4_address.sin_addr.s_addr = htonl(INADDR_ANY); // all interfaces (0.0.0.0)
    ipv4_address.sin_port = htons(port);

    int ipv4_bind_result = bind(ipv4_socket_fd, (struct sockaddr *)&ipv4_address, sizeof(ipv4_address));

    if (ipv4_bind_result < 0) {
        log_error("UdpServer: Socket bind failed");
        log_errno();
        exit(0);
    }

    log_debug("UdpServer: Created");
}

UdpServer::~UdpServer() {
    close(ipv4_socket_fd);
    log_debug("UdpServer: Disposed");
}

void UdpServer::receive(uint8_t* buffer, size_t buffer_size, size_t& data_length) {
    log_debug("UdpServer: Waiting for message");

    struct sockaddr_in remote_address;
    socklen_t addrlen = sizeof(remote_address);

    ssize_t value = recvfrom(
            ipv4_socket_fd,
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


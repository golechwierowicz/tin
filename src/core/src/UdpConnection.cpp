#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include "UdpConnection.h"
#include "Logger.h"

UdpConnection::UdpConnection() {
    socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        throwErrno("socket() failed");
    }

    logDebug() << "UdpConnection: Socket created";
}

void UdpConnection::bind(uint16_t port) {
    struct sockaddr_in6 address;

    memset((char *)&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any; // all interfaces (0.0.0.0)
    address.sin6_port = htons(port);

    int bind_result = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));

    if (bind_result < 0) {
        throwErrno("bind() failed");
    }

    logDebug() << "UdpConnection: Bind succeded";
}

UdpConnection::~UdpConnection() {
    close(socket_fd);
    logDebug() << "UdpConnection: Disposed";
}

void UdpConnection::send(uint8_t* buffer, size_t buffer_size, const std::string& addr, in_port_t port) {
    struct sockaddr_in6 remote;

    // TODO: read address to struct

    auto send_result = sendto(socket_fd, buffer, buffer_size, 0, (struct sockaddr*)& remote, sizeof(remote));
    if(send_result < 0) {
        throwErrno("sendto() failed");
    }
}

void UdpConnection::receive(uint8_t* buffer, size_t buffer_size, size_t& data_length) {
    logDebug() << "UdpConnection: Waiting for message";

    struct sockaddr_in remote_address;
    socklen_t addrlen = sizeof(remote_address);

    ssize_t value = recvfrom(
            socket_fd,
            buffer,
            buffer_size,
            0, // FLAGS
            (struct sockaddr *)&remote_address, &addrlen);

    if(value < 0) {
        throwErrno("Message receive failed");
    } else {
        logDebug() << "UdpConnection: Message received";
        data_length = (size_t) value;
    }
}

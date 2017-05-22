#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <arpa/inet.h>
#include <netdb.h>
#include "UdpConnection.h"
#include "Logger.h"

UdpConnection::UdpConnection() {
    socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        raiseError("socket() failed");
    }

    logDebug() << "UdpConnection: Socket created";
}

void UdpConnection::bind_port(uint16_t port)  {
    struct sockaddr_in6 address;

    memset((char *)&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any; // all interfaces (0.0.0.0)
    address.sin6_port = htons(port);

    int bind_result = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));

    if (bind_result < 0) {
        raiseError("bind() failed");
    }

    logDebug() << "UdpConnection: Bind succeded";
}

UdpConnection::~UdpConnection() {
    close(socket_fd);
    logDebug() << "UdpConnection: Disposed";
}

void UdpConnection::send(uint8_t* buffer, size_t buffer_size, const std::string& addr, in_port_t port) {
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    struct sockaddr_in6 sa6;
    sa6.sin6_family = AF_INET6;
    sa6.sin6_port = htons(port);

    bool ipv4 = true;

    auto p_res = inet_pton(AF_INET, addr.c_str(), &sa.sin_addr);
    if(p_res != 1) {
        ipv4 = false;
        p_res = inet_pton(AF_INET6, addr.c_str(), &sa6.sin6_addr);
        if(p_res != 1) {
            raiseError("inet_pton() failed");
        }
    }

    ssize_t s_res;
    if(ipv4) {
        logDebug() << "UdpConnection: Sending via ipv4";
        s_res = sendto(socket_fd, buffer, buffer_size, 0, (sockaddr*) &sa, sizeof(sa));
    } else {
        logDebug() << "UdpConnection: Sending via ipv6";
        s_res = sendto(socket_fd, buffer, buffer_size, 0, (sockaddr*) &sa6, sizeof(sa6));
    }

    if(s_res < 0) {
        raiseError("sendto() failed");
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
        raiseError("Message receive failed");
    } else {
        logDebug() << "UdpConnection: Message received";
        data_length = (size_t) value;
    }
}

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <arpa/inet.h>
#include "UdpConnection.h"
#include "Logger.h"

std::string UdpConnection::LOCALHOST("127.0.0.1");

std::string sockaddr_4or6::toString() {
    char* addr[200];
    if(ipv4) {
        inet_ntop(AF_INET, &addr4, (char*)addr, sizeof(addr4));
    } else {
        inet_ntop(AF_INET6, &addr6, (char*)addr, sizeof(addr6));
    }

    std::stringstream ss;
    ss << "Sockaddr4or6 ["
       << (ipv4 ? "ipv4" : "ipv6")
       << ", " << addr << "]";
    return ss.str();
}


void UdpConnection::open_socket() {
    socket_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        raiseError("socket() failed");
    }
    open = true;
    logDebug() << "UdpConnection: Socket opened";
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

    logDebug() << "UdpConnection: Socket bound";
}

void UdpConnection::close_socket() {
    if(open) {
        open = false;
        close(socket_fd);
        logDebug() << "UdpConnection: Socket closed";
    }
}

UdpConnection::~UdpConnection() {
    close_socket();
}

sockaddr_4or6 UdpConnection::getAddress(std::string addr, in_port_t port) {
    sockaddr_4or6 address;

    address.ipv4 = true;
    address.addr4.sin_family = AF_INET;
    address.addr4.sin_port = htons(port);
    address.addr6.sin6_family = AF_INET6;
    address.addr6.sin6_port = htons(port);

    auto p_res = inet_pton(AF_INET, addr.c_str(), &address.addr4.sin_addr);
    if(p_res != 1) {
        address.ipv4 = false;
        p_res = inet_pton(AF_INET6, addr.c_str(), &address.addr6.sin6_addr);
        if(p_res != 1) {
            raiseError("inet_pton() failed");
        }
    }

    return address;
}


void UdpConnection::send_msg(uint8_t *buffer, size_t len, sockaddr_4or6 &address) {
    ssize_t s_res;
    if(address.ipv4) {
        logDebug() << "UdpConnection: Sending via ipv4";
        s_res = sendto(socket_fd, buffer, len, 0, (sockaddr*) &address.addr4, sizeof(address.addr4));
    } else {
        logDebug() << "UdpConnection: Sending via ipv6";
        s_res = sendto(socket_fd, buffer, len, 0, (sockaddr*) &address.addr6, sizeof(address.addr6));
    }

    if(s_res < 0) {
        raiseError("sendto() failed");
    }
}

sockaddr_4or6 UdpConnection::receive(uint8_t* buffer, size_t buffer_size, size_t& data_length) {
    logDebug() << "UdpConnection: Waiting for message";

    struct sockaddr_in6 remote_address;
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

    sockaddr_4or6 sender;
    sender.ipv4 = false; // ipv4 addresses are mapped as https://en.wikipedia.org/wiki/IPv6#IPv4-mapped_IPv6_addresses
    sender.addr6 = remote_address;
    return sender;
}
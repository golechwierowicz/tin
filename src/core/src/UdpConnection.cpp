#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <arpa/inet.h>
#include "UdpConnection.h"
#include "Logger.h"

const std::string UdpConnection::LOCALHOST = "127.0.0.1";

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

void UdpConnection::send_msg(uint8_t *buffer, size_t len, sockaddr_storage address) {
    ssize_t result;
    if (address.ss_family == AF_INET) {
        result = sendto(socket_fd, buffer, len, 0, (sockaddr*)&address, sizeof(sockaddr_in));
    } else {
        result = sendto(socket_fd, buffer, len, 0, (sockaddr*)&address, sizeof(sockaddr_in6));
    }

    if(result < 0) {
        raiseError("sendto() failed");
    } else {
        logDebug() << "UdpConnection: Message sent to: " << addressStr(address);
    }
}

sockaddr_storage UdpConnection::receive(uint8_t* buffer, size_t buffer_size, size_t& data_length) {
    logDebug() << "UdpConnection: Waiting for message";

    sockaddr_storage sender;
    socklen_t addrlen = sizeof(sender);

    ssize_t value = recvfrom(
            socket_fd,
            buffer,
            buffer_size,
            0, // FLAGS
            (struct sockaddr *)&sender, &addrlen);

    if(value < 0) {
        raiseError("Message receive failed");
    } else {
        logDebug() << "UdpConnection: Message received";
        data_length = (size_t) value;
    }

    return sender;
}

sockaddr_storage UdpConnection::getAddress(const std::string& addr, in_port_t port) {
    sockaddr_storage address;
    sockaddr_in* addr4 = (sockaddr_in*) &address;
    sockaddr_in6* addr6 = (sockaddr_in6*) &address;

    addr4->sin_family = AF_INET;
    addr4->sin_port = htons(port);
    auto p_res = inet_pton(AF_INET, addr.c_str(), &addr4->sin_addr);

    if(p_res != 1) {
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = htons(port);
        p_res = inet_pton(AF_INET6, addr.c_str(), &addr6->sin6_addr);

        if(p_res != 1) {
            raiseError("inet_pton() failed");
        }
    }

    return address;
}

std::string UdpConnection::addressStr(sockaddr_storage& address) {
    char* addr[200];
    bool ipv4 = address.ss_family == AF_INET;

    if(ipv4) {
        inet_ntop(AF_INET, &((sockaddr_in*) &address)->sin_addr, (char*)addr, sizeof(sockaddr_in));
    } else {
        inet_ntop(AF_INET6, &((sockaddr_in6*) &address)->sin6_addr, (char*)addr, sizeof(sockaddr_in6));
    }

    in_port_t port = ntohs(ipv4 ?
       ((sockaddr_in*) &address)->sin_port :
       ((sockaddr_in6*) &address)->sin6_port
    );

    std::stringstream ss;
    ss << "sockaddr_storage ["
       << (ipv4 ? "ipv4" : "ipv6")
       << ", " << (const char*)addr
       << ", " << port
       << "]";
    return ss.str();
}

void UdpConnection::setAddrPort(sockaddr_storage& address, in_port_t port) {
    bool ipv4 = address.ss_family == AF_INET;

    if(ipv4) {
        ((sockaddr_in*) &address)->sin_port = htons(port);
    } else {
        ((sockaddr_in6*) &address)->sin6_port = htons(port);
    }
}

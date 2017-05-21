#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "UdpServer.h"
#include "logging.h"

UdpServer::UdpServer(uint16_t port) {
    ipv4_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (ipv4_socket_fd < 0) {
        log_error("Socket creation failed");
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
        log_error("Socket bind failed");
        log_errno();
        exit(0);
    }

    log_debug("UdpServer: Created");
}

UdpServer::~UdpServer() {
    log_debug("UdpServer: Disposed");
    close(ipv4_socket_fd);
}

void UdpServer::recieve(uint8_t* buffer, uint16_t& size) {
    log_debug("UdpServer: Waiting for message");
    size = 0;
    sleep(3);
}


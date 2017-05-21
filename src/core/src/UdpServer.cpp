#include <unistd.h>
#include "UdpServer.h"
#include "logging.h"

UdpServer::UdpServer() {
    log_debug("UdpServer: Created");
}

UdpServer::~UdpServer() {
    log_debug("UdpServer: Disposed");

}

void UdpServer::recieve(uint8_t* buffer, uint16_t& size) {
    log_debug("UdpServer: Waiting for message");
    size = 0;
    sleep(3);
}


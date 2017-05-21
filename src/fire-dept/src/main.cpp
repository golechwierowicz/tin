#include <UdpServer.h>
#include "logging.h"

int main() {
    log("Starting Fire Department Server");

    bool running = true;
    UdpServer server;

    uint8_t message_buffer[2048];
    uint16_t message_size;

    log("Fire Department Server listening");

    while(running) {
        server.recieve(message_buffer, message_size);
        log_debug("Message recieved");
    }
}

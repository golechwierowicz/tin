#include <UdpServer.h>
#include <Deserializer.h>
#include "logging.h"

#define BUFFER_SIZE 2048

int main() {
    log("Starting Fire Department Server");

    bool running = true;

    UdpServer server;
    try {
        server.open(1234);
    } catch(const std::runtime_error& error) {
        log_error(error);
        exit(1);
    }

    uint8_t message_buffer[BUFFER_SIZE];
    size_t message_size;

    log("Fire Department Server listening");

    while(running) {
        server.receive(message_buffer, BUFFER_SIZE - 1, message_size);

        Deserializer deserializer(message_buffer, (uint32_t) message_size);

        log_debug(message_buffer);
    }
}

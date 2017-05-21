#include <UdpServer.h>
#include "logging.h"

#define BUFFER_SIZE 2048

int main() {
    log("Starting Fire Department Server");

    bool running = true;
    UdpServer server(1234);

    uint8_t message_buffer[BUFFER_SIZE];
    size_t message_size;

    log("Fire Department Server listening");

    while(running) {
        server.receive(message_buffer, BUFFER_SIZE - 1, message_size);
        message_buffer[message_size] = 0; // terminate ascii string
        log_debug(message_buffer);
    }
}

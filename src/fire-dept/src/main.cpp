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
        server.receive(message_buffer, BUFFER_SIZE, message_size);
        log_debug("Message recieved");
    }
}

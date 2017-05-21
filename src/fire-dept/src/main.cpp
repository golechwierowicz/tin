#include <UdpServer.h>
#include <Deserializer.h>
#include <blocks/AbstractBlock.h>
#include <blocks/DebugBlock.h>
#include "logging.h"

#define BUFFER_SIZE 2048

void handle_message(uint8_t* message_buffer, size_t message_size) {
    Deserializer deserializer(message_buffer, (uint32_t) message_size);

    while (deserializer.next_block()) {
        switch (deserializer.get_block_type()) {
            case bt_debug: {
                DebugBlock block;
                block.deserialize(deserializer);
                auto str = block.toString();
                log("Received message");
                log(str);
            }
                break;
            default: {
                log("Received an unknown message");
            }
                break;
        }
    }
}

int main() {
    log("Starting Fire Department Server");

    bool running = true;

    UdpServer server;
    try {
        server.open(1234);
    } catch (const std::runtime_error& error) {
        log_exception(error);
        exit(1);
    }

    uint8_t message_buffer[BUFFER_SIZE];
    size_t message_size;

    log("Fire Department Server listening");

    while (running) {
        try {
            server.receive(message_buffer, BUFFER_SIZE - 1, message_size);
            handle_message(message_buffer, message_size);

        } catch (const std::runtime_error& error) {
            log_exception(error);
            continue;
        }
    }
}

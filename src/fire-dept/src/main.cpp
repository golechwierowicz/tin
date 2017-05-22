#include <UdpConnection.h>
#include <Deserializer.h>
#include <blocks/AbstractBlock.h>
#include <blocks/BlockReader.h>
#include "Logger.h"

#define BUFFER_SIZE 2048

void handle_message(uint8_t* message_buffer, size_t message_size) {
    BlockReader reader(message_buffer, message_size);

    for(AbstractBlock* block : reader.blocks) {
        log() << "Message: " << block->toString();
    }
}

int main() {
    log() << "Starting Fire Department Server";

    bool running = true;

    UdpConnection server;

    try {
        server.open_socket();
        server.bind_port(1234);
    } catch (const std::runtime_error& e) {
        logError() << e.what();
        exit(1);
    }

    uint8_t message_buffer[BUFFER_SIZE];
    size_t message_size;

    log() << "Fire Department Server listening";

    while (running) {
        try {
            auto addr = server.receive(message_buffer, BUFFER_SIZE - 1, message_size);
            logDebug() << "Message from: " << addr.toString();
            handle_message(message_buffer, message_size);

        } catch (const std::runtime_error& e) {
            logError() << e.what();
            continue;
        }
    }
}

#include <UdpConnection.h>
#include <Deserializer.h>
#include <blocks/AbstractBlock.h>
#include <blocks/BlockReader.h>
#include <FireDeptConfig.h>
#include <blocks/CentralServerHeartbeat.h>
#include <blocks/CentralServerFireAlert.h>
#include "Logger.h"

#define BUFFER_SIZE 2048

void handle_message(uint8_t* message_buffer, size_t message_size) {
    BlockReader reader(message_buffer, message_size);

    for(auto& block : reader.blocks) {
        if(block->type == BlockType::central_server_heartbeat) {
            auto heartbeatBlock = reinterpret_cast<CentralServerHeartbeat*>(block.get());
            log() << "Heartbeat: " << heartbeatBlock->toString();
        } else if(block->type == BlockType::central_server_fire_alert) {
            auto fireAlertBlock = reinterpret_cast<CentralServerFireAlert*>(block.get());
            log() << "FIRE!: " << fireAlertBlock->toString();
        }
    }
}

int main(int argc, char *argv[]) {
    FireDeptConfig config;
    if(argc == 2) {
        log() << "Reading config from file";
        config = FireDeptConfig(argv[1]);
    }

    log() << "Starting Fire Department Server";

    bool running = true;

    UdpConnection server;

    try {
        server.open_socket();
        server.bind_port(config.getPort());
    } catch (const std::runtime_error& e) {
        logError() << e.what();
        exit(1);
    }

    uint8_t message_buffer[BUFFER_SIZE];
    size_t message_size;

    log() << "Fire Department Server listening on: " << config.getPort();

    while (running) {
        try {
            auto address = server.receive(message_buffer, BUFFER_SIZE - 1, message_size);
            logDebug() << "Message from: " << UdpConnection::address_to_str(address);
            handle_message(message_buffer, message_size);

        } catch (const std::runtime_error& e) {
            logError() << e.what();
            continue;
        }
    }
}

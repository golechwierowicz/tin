#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <Serializer.h>
#include <Sensor.h>
#include <Logger.h>

static const int BUF_SIZE = 2048;
static const int NUMBER_OF_ATTEMPTS = 5;
using namespace std;
volatile bool quit = false;
uint8_t buf[BUF_SIZE];

Serializer serializer;
Sensor sensor(serializer);

void signal_handler( int signum ) {
    cout << "Received: " << signum << endl;
    sensor.close_connection();
    cout << "Closed socket..." << endl;
    quit = true;
    exit(signum);
}

void config_receive_wrapper() {
    while(!quit) {
        sensor.receive_cc_config_msg();
    }
}

int main() {
    signal(SIGINT, signal_handler);
    bool received_initial_config = false;
    sensor.set_connection_timeout(1, 0);
    for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) {
        sensor.send_request_msg();
        received_initial_config = sensor.receive_cc_config_msg();
        if (received_initial_config) {
            log() << "Received initial configuration\n";
            sensor.unset_connection_timeout();
            break;
        }
    }
    if (!received_initial_config) {
        logError() << "Failed to receive initial configuration\n";
        exit(-1);
    }

    std::thread receiving_thread(config_receive_wrapper);
    while(!quit) {
        sensor.broadcast_centrals();
        this_thread::sleep_for(std::chrono::seconds(2));
    }
    exit(0);
}

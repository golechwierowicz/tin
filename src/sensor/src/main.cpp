#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <Serializer.h>
#include <Sensor.h>
#include <Deserializer.h>
#include <Logger.h>

static const int BUF_SIZE = 2048;
static const int NUMBER_OF_ATTEMPTS = 5;
using namespace std;
bool quit = false;

Serializer serializer;
Sensor sensor(serializer);

void signal_handler( int signum ) {
    cout << "Received: " << signum << endl;
    sensor.close_connection();
    cout << "Closed socket..." << endl;
    quit = true;
    exit(signum);
}

int main() {
    signal(SIGINT, signal_handler);
    uint8_t buf[BUF_SIZE];
    bool received_initial_config = false;
    sensor.set_connection_timeout(1, 0);
    for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) {
        sensor.send_request_msg();
        received_initial_config = sensor.receive_cc_config_msg(buf, BUF_SIZE);
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
    // to be put in a separate thread; another thread will send measurements to local stations
    while(!quit) {
        sensor.receive_cc_config_msg(buf, BUF_SIZE);
        this_thread::sleep_for(std::chrono::seconds(2));
    }
    exit(0);
}

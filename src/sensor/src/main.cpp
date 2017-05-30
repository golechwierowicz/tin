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

namespace {
    Sensor* g_sensor;
}

void signal_handler( int signum ) {
    cout << "Received: " << signum << endl;
    g_sensor->close_connection();
    cout << "Closed socket..." << endl;
    quit = true;
    exit(signum);
}

void config_receive_wrapper() {
    while(!quit) {
        g_sensor->receive_cc_config_msg();
    }
}

int main(int argc, const char** argv) {
    if(argc < 2) {
        log() << "Invalid args. Usage: ./sensor [path_to_config] \n"
              << "(supposing the executable is in your current directory\n "
              << "in short: the path to config is the only command line argument)";
        exit(-1);
    }

    Serializer serializer;
    Sensor sensor(serializer, argv[1]);
    g_sensor = &sensor;

    signal(SIGINT, signal_handler);
    bool received_initial_config = false;
    g_sensor->set_connection_timeout(1, 0);
    for (int i = 0; i < NUMBER_OF_ATTEMPTS; i++) {
        g_sensor->send_request_msg();
        received_initial_config = g_sensor->receive_cc_config_msg();
        if (received_initial_config) {
            log() << "Received initial configuration\n";
            g_sensor->unset_connection_timeout();
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
    receiving_thread.join();
    exit(0);
}

#include <iostream>
#include <ControlCenter.h>
#include <Serializer.h>
#include <Logger.h>
#include <ConfigReader.h>
#include <csignal>
#include <chrono>
#include <thread>

using namespace std;

namespace {
    static const int8_t BROADCAST_SENSORS_INTERVAL = 3;
    ControlCenter *g_cc;
}

volatile bool quit = false;

void signal_handler( int signum ) {
    log() << "Received: " << signum;
    g_cc->close_connection();
    quit = true;
    exit(signum);
}

void receive_sensor_request() {
    while(!quit) {
        g_cc->recv_sensor_request_msg(); // it hangs on this until it receives next message
    }
}

int main(int argc, const char** argv) {
    if(argc < 2) {
        log() << "Invalid args. Usage: ./control-center [path_to_config]";
        exit(-1);
    } else {
        Serializer s;
        std::string cfg_filepath = std::string(argv[1]);
        ControlCenter cc(s, cfg_filepath);

        g_cc = &cc;

        signal(SIGINT, signal_handler);
        
        std::thread receive_config_request_thread(receive_sensor_request);

        while(!quit) {
            g_cc->broadcast_sensors();
            this_thread::sleep_for(std::chrono::seconds(BROADCAST_SENSORS_INTERVAL));
        }

        receive_config_request_thread.join();
        exit(0);
    }
}

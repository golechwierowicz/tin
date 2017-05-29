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
    ControlCenter *g_cc;
}

volatile bool quit = false;

void signal_handler( int signum ) {
    log() << "Received: " << signum;
    g_cc->close_connection();
    quit = true;
    exit(signum);
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

        // TODO: move this to another thread and put it in a loop.
        cc.recv_sensor_request_msg();
        while(!quit) {
            cc.broadcast_sensors();
            this_thread::sleep_for(std::chrono::seconds(3));
        }
        exit(0);
    }
}

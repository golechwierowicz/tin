#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <Serializer.h>
#include <Sensor.h>
#include <Deserializer.h>

static const int BUF_SIZE = 2048;
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
    while(!quit) {
        sensor.send_request_msg();
        sensor.receive_cc_config_msg(buf, BUF_SIZE);
        this_thread::sleep_for(std::chrono::seconds(2));
    }
    exit(0);
}

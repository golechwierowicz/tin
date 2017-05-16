#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <Serializer.h>
#include <Sensor.h>

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
    //signal(SIGINT, signal_handler);

//    while(!quit) {
//        sensor.send_test_msg(); // this will be ping in the future
//        // we will need to think how to handle the sleep/tim between send/recv calls and not missing conf packets from CC
//        this_thread::sleep_for(std::chrono::seconds(2));
//        sensor.send_request_msg();
//        sensor.receive_cc_config_msg();
//    }
    sensor.send_request_msg();
    sensor.receive_cc_config_msg();
    //return 0;
    exit(0);
}

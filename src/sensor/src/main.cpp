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
    signal(SIGINT, signal_handler);

    while(!quit) {
        sensor.send_test_msg(); // this will be ping in the future
        this_thread::sleep_for(std::chrono::seconds(2));
        sensor.receive_cc_test_msg();
    }

    return 0;
}

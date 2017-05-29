#include <iostream>
#include <ControlCenter.h>
#include <Serializer.h>
#include <csignal>
#include <chrono>
#include <thread>

using namespace std;

Serializer s;
ControlCenter cc(s);
bool quit = false;

void signal_handler( int signum ) {
    cout << "Received: " << signum << endl;
    cc.close_connection();
    cout << "Closed socket..." << endl;
    quit = true;
    exit(signum);
}

int main() {
    signal(SIGINT, signal_handler);

    // na razie tutaj, zeby zarejestrowc jeden sensor, potem do osobnego watku chyba
    cc.recv_sensor_request_msg();
	while(!quit) {
        cc.broadcast_sensors();
        this_thread::sleep_for(std::chrono::seconds(3));
	}
    exit(0);
}

#include <iostream>
#include <ControlCenter.h>
#include <Serializer.h>
#include <csignal>

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

	while(!quit) {
		cc.recv_test_sensor_msg();	
        cc.broadcast_sensors();
	}

	return 0;
}

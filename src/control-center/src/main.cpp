#include <iostream>
#include <ControlCenter.h>
#include <csignal>

using namespace std;

ControlCenter cc;
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
	}

	return 0;
}

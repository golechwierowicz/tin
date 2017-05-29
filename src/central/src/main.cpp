#include "Central.h"
#include <csignal>
#include <iostream>

namespace {
    Central * volatile g_central;
}

void sigint_handler(int signum) {
    std::cout << "Received: " << signum << std::endl;
    g_central->shutdown();
}

int main(int argc, char *argv[]) {
    CentralConfig config;
    config.heartbeat_interval_ms = 5000;
    config.alert_interval_ms = 1000;
    config.fire_dept_ip = "127.0.0.1";
    config.fire_dept_port = 4099;
    config.port = 4011;
    config.id = 1;

    Central central(config);
    g_central = &central;
    signal(SIGINT, sigint_handler);

    central.run();
}
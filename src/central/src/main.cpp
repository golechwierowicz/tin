#include "Central.h"
#include <csignal>
#include <iostream>

namespace {
    volatile Central *g_central;
}

void sigint_handler(int signum) {
    std::cout << "Received: " << signum << std::endl;
    g_central->shutdown();
}

int main(int argc, char *argv[]) {
    CentralConfig config;
    config.heartbeet_interval_ms = 5000;
    config.alert_interval_ms = 1000;

    Central central(config);
    g_central = &central;
    signal(SIGINT, sigint_handler);

    central.run();
}
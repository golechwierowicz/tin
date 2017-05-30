#include "Central.h"
#include <csignal>
#include <iostream>
#include "ConfigReader.h"

namespace {
    Central * volatile g_central;
}

void sigint_handler(int signum) {
    std::cout << "Received: " << signum << std::endl;
    g_central->shutdown();
}

int main(int argc, char *argv[]) {
    CentralConfig config;

    if (argc < 2) {
        std::cerr << "Invalid number of arguments, usage: " << argv[0] << " <path-to-config>" << std::endl;
        std::cerr << "Loading default config, for development" << std::endl;

        config.heartbeat_interval_ms = 5000;
        config.alert_interval_ms = 1000;
        config.fire_dept_ip = "127.0.0.1";
        config.fire_dept_port = 4099;
        config.port = 4011;
        config.id = 1;
    } else {
        ConfigReader reader(argv[1]);
        config.heartbeat_interval_ms = static_cast<uint32_t>(reader.read_integer("heartbeat_interval_ms"));
        config.alert_interval_ms = static_cast<uint32_t>(reader.read_integer("alert_interval_ms"));
        config.fire_dept_ip = reader.read_string("fire_dept_ip");
        config.fire_dept_port = static_cast<uint16_t>(reader.read_integer("fire_dept_port"));
        config.port = static_cast<uint16_t>(reader.read_integer("port"));
        config.id = static_cast<uint32_t>(reader.read_integer("id"));
    }

    Central central(config);
    g_central = &central;
    signal(SIGINT, sigint_handler);

    central.run();
}
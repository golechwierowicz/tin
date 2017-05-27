#pragma once

#include <thread>
#include <atomic>
#include <shared_mutex>
#include <chrono>
#include "CentralConfig.h"
#include "UdpConnection.h"

class Central {
    CentralConfig config;
    std::shared_mutex config_lock;

    UdpConnection connection;
    std::shared_mutex connection_lock;

    std::thread listen_thread;
    std::atomic_bool running;

    std::chrono::time_point<std::chrono::system_clock> last_heartbeat;

public:
    explicit Central(const CentralConfig &config);

    void reload_config(const CentralConfig &config);
    void shutdown() volatile;

    void run();

private:
    void listen();

    void apply_port_change();

    std::chrono::milliseconds getHeartbeatInterval();
    std::chrono::milliseconds getAlertInterval();

    void send_hearbeat();
};

#pragma once

#include <thread>
#include <atomic>
#include <shared_mutex>
#include <chrono>
#include "CentralConfig.h"
#include "UdpConnection.h"
#include "AlertAggregator.h"

/**
 * This class is thread-safe.
 */
class Central {
    CentralConfig config;
    std::shared_mutex config_lock;

    UdpConnection connection;
    std::shared_mutex connection_lock;

    std::thread listen_thread;
    std::thread alert_send_thread;
    std::atomic_bool running;

    std::chrono::time_point<std::chrono::system_clock> last_heartbeat;

    AlertAggregator alert_aggregator;

public:
    explicit Central(const CentralConfig &config);

    void reload_config(const CentralConfig &config);
    void shutdown() volatile;

    void run();

private:
    void listen();
    void run_sending_alerts();

    void handle_block(const SensorCommonBlock &common, const std::unique_ptr<AbstractBlock> &block);

    void send_heartbeat();

    void init_socket();
    void init_socket_impl();
    void apply_port_change();

    std::chrono::milliseconds get_heartbeat_interval();
    std::chrono::milliseconds get_alert_interval();
    int32_t get_id();
    sockaddr_storage get_fire_dept_address();
};
#include "Central.h"
#include <mutex>

using namespace std;

Central::Central(const CentralConfig &config) : config(config) {
}

void Central::reload_config(const CentralConfig &config) {
    auto prev_config = this->config;

    {
        shared_lock<shared_mutex> lock(config_lock);
        this->config = config;
    }

    if (prev_config.port != this->config.port) {
        apply_port_change();
    }
}

void Central::shutdown() volatile {
    running.store(false, memory_order_release);
}

void Central::run() {
    last_heartbeat = std::chrono::time_point<chrono::system_clock>::min();
    running.store(true, memory_order_release);
    listen_thread = thread(bind(&Central::listen, this));

    while (running.load(memory_order_consume)) {
        const auto since_last_heartbeat = std::chrono::system_clock::now() - last_heartbeat;
        const chrono::milliseconds heartbeat_interval = getHeartbeatInterval();

        if (since_last_heartbeat >= heartbeat_interval) {
            send_hearbeat();
        }
        
        const auto wait_duration = heartbeat_interval - since_last_heartbeat;
        this_thread::sleep_for(wait_duration);
    }
}

void Central::listen() {
    while (running.load(memory_order_consume)) {

    }
}

void Central::apply_port_change() {
    unique_lock<shared_mutex> lock(connection_lock);
}

std::chrono::milliseconds Central::getHeartbeatInterval() {
    shared_lock<shared_mutex> lock(config_lock);
    return chrono::milliseconds(config.heartbeet_interval_ms);
}

std::chrono::milliseconds Central::getAlertInterval() {
    shared_lock<shared_mutex> lock(config_lock);
    return chrono::milliseconds(config.alert_interval_ms);
}

void Central::send_hearbeat() {

}

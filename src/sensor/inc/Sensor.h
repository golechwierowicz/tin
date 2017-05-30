#ifndef _SENSOR_H
#define _SENSOR_H

#include <sys/socket.h>
#include <cstdio>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <Serializer.h>
#include <SensorConfig.h>
#include <string>
#include <arpa/inet.h>
#include <vector>
#include <UdpConnection.h>
#include <ConfigReader.h>
#include <mutex>
#include "SensorConfig.h"

class Sensor {
private:
    static const int BUF_SIZE = 512;
    Serializer serializer;
    UdpConnection con_send;
    UdpConnection con_recv;

    SensorConfig config;
    ConfigReader config_reader;
    uint8_t buf[BUF_SIZE];

    std::mutex mutex;

    void init_recv_connection();
    void reload_config(std::string& cc_ip, in_port_t cc_port, std::vector<std::string>& central_ips);

public:
    Sensor(Serializer serializer);
    ~Sensor();

    void send_request_msg();
    bool receive_cc_config_msg();
    void close_connection();
    void set_connection_timeout(long int sec, long int microsec);
    void unset_connection_timeout();
    void send_measurement(std::string central_ip, in_port_t port);
    void broadcast_centrals();
};

#endif // _SENSOR_H

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
#include "SensorConfig.h"

class Sensor {
private:
    Serializer serializer;
    UdpConnection con_send;
    UdpConnection con_recv;

    SensorConfig* config;
    std::string ip_address;
    in_port_t port = 4049;
    std::vector<std::string> central_ips;

    void init_recv_connection();
    void reload_config(in_port_t);
public:
    Sensor(Serializer serializer);
    ~Sensor();

    // communication cc - sensor
    void send_request_msg();
    void receive_cc_config_msg(uint8_t *buf, size_t bufSize);
    void close_connection();

    SensorConfig* init_config(); // consider using smart ptr.........
};

#endif // _SENSOR_H

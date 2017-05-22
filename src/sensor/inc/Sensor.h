#ifndef _SENSOR_H
#define _SENSOR_H

#include <sys/socket.h>
#include <cstdio>
#include <netdb.h>
#include <cstdlib>
#include <zconf.h>
#include <cstring>
#include <Serializer.h>
#include <AddressInfo.h>
#include <SensorConfig.h>
#include <string>
#include <arpa/inet.h>

#include "SensorConfig.h"
#include <vector>
#include <UdpConnection.h>

class Sensor {
private:
    Serializer serializer;
    UdpConnection con_send;
    UdpConnection con_recv;
    SensorConfig* config;
    AddressInfo* addrInfo;
    std::vector<std::string> central_ips;
    void init_recv_connection();
    void create_request_block();
    void reload_config(in_port_t);
public:
    Sensor(Serializer serializer);
    ~Sensor();

    // communication cc - sensor
    void send_test_msg();
    void send_request_msg();
    void receive_cc_config_msg();
    void close_connection();

    SensorConfig* init_config(); // consider using smart ptr.........
};

#endif // _SENSOR_H

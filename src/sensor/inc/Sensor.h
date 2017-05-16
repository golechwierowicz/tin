#ifndef _SENSOR_H
#define _SENSOR_H

#include <sys/socket.h>
#include <cstdio>
#include <netdb.h>
#include <cstdlib>
#include <zconf.h>
#include <cstring>
#include <Serializer.h>
#include <SensorConfig.h>
#include <string>
#include <arpa/inet.h>

#include "../../core/inc/Serializer.h"
#include "../../core/inc/Connection.h"
#include "SensorConfig.h"
#include <vector>

class Sensor {
private:
    Serializer serializer;
    Connection* con_send;
    Connection* con_recv;
    SensorConfig* config;
    in_port_t _default_CC_port;
    std::vector<std::string> central_ips;
    void init_recv_connection();
    void create_request_block();
public:
    Sensor(Serializer serializer);
    ~Sensor();

    // communication cc - sensor
    void reload_config();
    void send_test_msg();
    void send_request_msg();
    void receive_cc_config_msg();
    void close_connection();

    SensorConfig* init_config(); // consider using smart ptr.........
};

#endif // _SENSOR_H

#ifndef _SENSOR_H
#define _SENSOR_H

#include <sys/socket.h>
#include <cstdio>
#include <netdb.h>
#include <cstdlib>
#include <zconf.h>
#include <cstring>
#include <Serializer.h>
#include <Config.h>
#include <string>
#include <arpa/inet.h>

#include "../../core/inc/Serializer.h"
#include "../../core/inc/Connection.h"
#include "Config.h"
#include <ACKBlock.h>

class Sensor {
private:
    Serializer serializer;
    Connection* con_send;
    Connection* con_recv;
    Config* config;
    in_port_t _port;
    void init_recv_connection();
public:
    Sensor(Serializer serializer);
    ~Sensor();

    // communication cc - sensor
    void send_ack_to_cc(ACKBlock ackBlock);
    void reload_config();
    void send_test_msg();
    void receive_cc_test_msg();
    void close_connection();

    Config* init_config(); // consider using smart ptr.........
};

#endif // _SENSOR_H

#ifndef _CONTROL_CENTER_H_
#define _CONTROL_CENTER_H_

#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <zconf.h>
#include <cstring>
#include <arpa/inet.h>

#include "ConfigBlock.h"
#include "Serializer.h"
#include "Connection.h"
#include <vector>

class ControlCenter {
private:
    const static in_port_t port = 4048;
    std::vector<long> sensors;
    Connection* connection;
    Connection* con_send;
    Serializer _serializer;
public:
    ControlCenter(Serializer serializer);
    ~ControlCenter();
    void recv_test_sensor_msg();
    void send_test_sensor_msg();
    void close_connection();
    void init_connection();
    void init_sensor_connection(std::string sens_addr, in_port_t sens_port);
private:
    void update(ConfigBlock config_block);

};

#endif // _CONTROL_CENTER_H_

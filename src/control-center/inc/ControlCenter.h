#ifndef _CONTROL_CENTER_H_
#define _CONTROL_CENTER_H_

#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <zconf.h>
#include <cstring>
#include <arpa/inet.h>

#include "Serializer.h"
#include "Connection.h"
#include "AddressInfo.h"
#include <vector>

class ControlCenter {
private:
    const static in_port_t port = 4040;
    Connection* connection;
    Connection* con_send;
    Serializer _serializer;
    std::vector<AddressInfo*> _sensors;
public:
    ControlCenter(Serializer serializer);
    ~ControlCenter();
    void recv_test_sensor_msg();
    void recv_sensor_request_msg();
    void send_config_sensor_msg(in_port_t port, const char* addr);
    void close_connection();
    void init_connection();
    void broadcast_sensors();
private:
    void create_sensor_config_block(
            std::vector<std::string> central_ips,
            in_port_t port_id,
            std::string cnt_ip);
    void read_sensors();
    std::vector<std::string> get_central_ips();

};

#endif // _CONTROL_CENTER_H_

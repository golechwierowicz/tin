#ifndef _CONTROL_CENTER_H_
#define _CONTROL_CENTER_H_

static const int BUF_SIZE = 512;

#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>

#include "Serializer.h"
#include <vector>
#include <UdpConnection.h>
#include <map>

class ControlCenter {
private:
    in_port_t port = 4040;
    std::string ip = "127.0.0.1";
    uint8_t buf[BUF_SIZE];
    UdpConnection connection;
    UdpConnection con_send;
    Serializer serializer;

    std::map<uint32_t, sockaddr_storage> sensors;
    std::vector<std::string> get_central_ips();
    void update_sensor_list(uint32_t sensor_id, sockaddr_storage);
public:
    ControlCenter(Serializer serializer);
    ~ControlCenter();
    void recv_sensor_request_msg();
    void close_connection();
    void init_connection();
    void broadcast_sensors();

    void send_sensor_config_block(const sockaddr_storage &addr);
};

#endif // _CONTROL_CENTER_H_

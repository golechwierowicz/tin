#ifndef _CONTROL_CENTER_H_
#define _CONTROL_CENTER_H_

static const int BUF_SIZE = 512;

#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <zconf.h>
#include <cstring>
#include <arpa/inet.h>

#include "Serializer.h"
#include "AddressInfo.h"
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
    std::map<uint32_t, sockaddr_4or6> sensors;
    std::vector<std::string> get_central_ips();
    void update_sensor_list(uint32_t sensor_id, sockaddr_4or6);
public:
    ControlCenter(Serializer serializer);
    ~ControlCenter();
    void recv_sensor_request_msg();
    void close_connection();
    void init_connection();
};

#endif // _CONTROL_CENTER_H_

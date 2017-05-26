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

class ControlCenter {
private:
    in_port_t port = 4040;
    std::string ip = "127.0.0.1";
    uint8_t buf[BUF_SIZE];
    UdpConnection connection;
    UdpConnection con_send;
    Serializer serializer;
    std::vector<sockaddr_storage> sensors;
public:
    ControlCenter(Serializer serializer);
    ~ControlCenter();
    void recv_sensor_request_msg();
    void close_connection();
    void init_connection();
private:
    std::vector<std::string> get_central_ips();
    void update_sensor_list(sockaddr_storage);

};

#endif // _CONTROL_CENTER_H_

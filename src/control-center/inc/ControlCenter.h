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
#include <vector>

class ControlCenter {
private:
    std::vector<long> sensors;
public:
    ControlCenter();
    void recv_test_sensor_msg();
    void send_test_sensor_msg();
private:
    void update(ConfigBlock config_block);

};

#endif // _CONTROL_CENTER_H_

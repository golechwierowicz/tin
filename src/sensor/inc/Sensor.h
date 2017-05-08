#ifndef _SENSOR_H
#define _SENSOR_H
#include "ACKBlock.h"
#include "../../core/inc/Serializer.h"
#include "Config.h"

class Sensor {
private:
    Serializer serializer;


public:
    Sensor(Serializer serializer);
    Config* config;
    // communication central - sensor
    void send_ack_to_cc(ACKBlock ackBlock);
    void reload_config();
    void send_test_msg();
    void receive_test_msg();
    Config* init_config();
};

#endif // _SENSOR_H

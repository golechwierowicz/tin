#ifndef _SENSOR_H
#define _SENSOR_H
#include "ACKBlock.h"

class Sensor {
private:
    Serializer serializer;
    Config config;

public:
    Sensor(Serializer serializer): serializer(serializer);
     
    // communication central - sensor
    void send_ack_to_cc(ACKBlock ackBlock);
    void reload_config();
};

#endif // _SENSOR_H

#ifndef TIN_ACKBLOCK_H
#define TIN_ACKBLOCK_H

class ACKBlock {
    typedef long sensor_id_t;
    sensor_id_t sensor_id;

public:
    ACKBlock(sensor_id_t sensor_id);
};

#endif //TIN_ACKBLOCK_H

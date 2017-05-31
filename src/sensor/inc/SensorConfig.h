//
// Created by Grzegorz Olechwierowicz on 27.04.2017.
//

#ifndef TIN_CONFIG_H
#define TIN_CONFIG_H

#include <netinet/in.h>
#include <string>
#include <vector>

enum SensorType {
    st_smoke_sensor = 2,
    st_ir_sensor = 3,
    st_temp_sensor = 4
};

struct SensorConfig {
    const std::string SENSOR_PORT_PATH = "sensor.port";
    const std::string SENSOR_ID_PATH = "sensor.id";
    const std::string SENSOR_TYPE_PATH = "sensor.type";
    const std::string SENSOR_LONGITUDE = "sensor.longitude";
    const std::string SENSOR_LATITUDE = "sensor.latitude";
    const std::string DEFAULT_CC_PORT = "sensor.cc_port";
    const std::string DEFAULT_CC_IP = "sensor.cc_ip";

    uint32_t id;
    in_port_t sensor_port;
    uint16_t longitude;
    uint16_t latitude;
    SensorType type;

    in_port_t cc_port;
    std::string cc_addr;
    
    std::vector<std::string> central_ips;
};

#endif //TIN_CONFIG_H

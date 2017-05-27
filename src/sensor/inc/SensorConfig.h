//
// Created by Grzegorz Olechwierowicz on 27.04.2017.
//

#ifndef TIN_CONFIG_H
#define TIN_CONFIG_H

#include <netinet/in.h>
#include <string>

enum SensorType {
    st_smoke_sensor = 2,
    st_ir_sensor = 3,
    st_temp_sensor = 4
};

class SensorConfig {
private:
    // to be read from file
    uint32_t id;
    // to be read from file
    SensorType type;
    in_port_t cc_port;
    const std::string& cc_addr;

public:
    SensorConfig(SensorType type, in_port_t cc_port, const std::string& cc_addr) :
        type(type),
        cc_port(cc_port),
        cc_addr(cc_addr) {}

    uint32_t getId() const { return id; }
    void setId(uint32_t id) { SensorConfig::id = id; }
    SensorType getType() const { return type; }
    void setType(SensorType type) { SensorConfig::type = type; }
    in_port_t getCc_port() const { return cc_port; }
    void setCc_port(in_port_t cc_port) { SensorConfig::cc_port = cc_port; }
    const std::string& getCc_addr() const { return cc_addr; }
};

#endif //TIN_CONFIG_H

//
// Created by Grzegorz Olechwierowicz on 27.04.2017.
//

#ifndef TIN_CONFIG_H
#define TIN_CONFIG_H

#include <netinet/in.h>
#include <string>

enum SensorType {
    smoke_sensor = 2,
    ir_sensor = 3,
    temp_sensor = 4
};

class SensorConfig {
public:
    SensorConfig(SensorType type, in_port_t cc_port, std::string cc_addr);
    ~SensorConfig();

    in_port_t getCc_port() const;
    void setCc_port(in_port_t cc_port);
    const std::string &getCc_addr() const;
    void setCc_addr(const std::string &cc_addr);

    SensorType getType() const;
    uint32_t getId() const;

private:
    // to be read from file
    uint32_t id;
    // to be read from file
    SensorType type;

    in_port_t cc_port;
    std::string cc_addr;
};

#endif //TIN_CONFIG_H

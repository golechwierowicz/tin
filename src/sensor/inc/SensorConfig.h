//
// Created by Grzegorz Olechwierowicz on 27.04.2017.
//

#ifndef TIN_CONFIG_H
#define TIN_CONFIG_H

#include <netinet/in.h>
#include <string>

class SensorConfig {
public:
    SensorConfig(
            int heat_limit,
            int smoke_limit,
            int infrared_limit,
            in_port_t cc_port,
            const std::string& cc_addr) :
        heat_limit(heat_limit),
        smoke_limit(smoke_limit),
        infrared_limit(infrared_limit),
        cc_port(cc_port),
        cc_addr(cc_addr) {}

    int heat_limit;
    int smoke_limit;
    int infrared_limit;
    in_port_t cc_port;
    const std::string& cc_addr;
};

#endif //TIN_CONFIG_H

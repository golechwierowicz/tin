//
// Created by Grzegorz Olechwierowicz on 27.04.2017.
//

#ifndef TIN_CONFIG_H
#define TIN_CONFIG_H

#include <netinet/in.h>

class Config {
public:
    Config(int heat_limit, int smoke_limit, int infrared_limit, in_port_t cc_port);

    in_port_t getCc_port() const;
    void setCc_port(in_port_t cc_port);
    int getHeat_limit() const;
    void setHeat_limit(int heat_limit);
    int getSmoke_limit() const;
    void setSmoke_limit(int smoke_limit);
    int getInfrared_limit() const;
    void setInfrared_limit(int infrared_limit);

private:
    int heat_limit;
    int smoke_limit;
    int infrared_limit;
    in_port_t cc_port;
};

#endif //TIN_CONFIG_H

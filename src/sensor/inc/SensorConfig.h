//
// Created by Grzegorz Olechwierowicz on 27.04.2017.
//

#ifndef TIN_CONFIG_H
#define TIN_CONFIG_H

#include <netinet/in.h>
#include <string>

class SensorConfig {
public:
    SensorConfig(int heat_limit, int smoke_limit, int infrared_limit, in_port_t cc_port, std::string cc_addr);
    ~SensorConfig();

    in_port_t getCc_port() const;
    void setCc_port(in_port_t cc_port);
    int getHeat_limit() const;
    void setHeat_limit(int heat_limit);
    int getSmoke_limit() const;
    void setSmoke_limit(int smoke_limit);
    int getInfrared_limit() const;
    void setInfrared_limit(int infrared_limit);
    std::string getCC_Addr() const;
    void setCC_Addr(int infrared_limit);

private:
    int heat_limit;
    int smoke_limit;
    int infrared_limit;
    in_port_t cc_port;
    std::string cc_addr;
};

#endif //TIN_CONFIG_H

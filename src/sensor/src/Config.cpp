#include <Config.h>

//
// Created by igor on 08.05.17.
//
Config::Config(int heat_limit, int smoke_limit, int infrared_limit, in_port_t cc_port) :
    heat_limit(heat_limit), smoke_limit(smoke_limit), infrared_limit(infrared_limit), cc_port(cc_port) {}

in_port_t Config::getCc_port() const {
    return cc_port;
}

void Config::setCc_port(in_port_t cc_port) {
    Config::cc_port = cc_port;
}

int Config::getHeat_limit() const {
    return heat_limit;
}

void Config::setHeat_limit(int heat_limit) {
    Config::heat_limit = heat_limit;
}

int Config::getSmoke_limit() const {
    return smoke_limit;
}

void Config::setSmoke_limit(int smoke_limit) {
    Config::smoke_limit = smoke_limit;
}

int Config::getInfrared_limit() const {
    return infrared_limit;
}

void Config::setInfrared_limit(int infrared_limit) {
    Config::infrared_limit = infrared_limit;
}



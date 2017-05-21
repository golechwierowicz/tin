#include <SensorConfig.h>
#include <cstdio>

//
// Created by igor on 08.05.17.
// TODO: remove get/set because it is just a simple dto...
//
SensorConfig::SensorConfig(int heat_limit, int smoke_limit, int infrared_limit, in_port_t cc_port, std::string cc_addr) :
    heat_limit(heat_limit), smoke_limit(smoke_limit), infrared_limit(infrared_limit), cc_port(cc_port), cc_addr(cc_addr) {}

SensorConfig::~SensorConfig() {}

in_port_t SensorConfig::getCc_port() const {
    return cc_port;
}

void SensorConfig::setCc_port(in_port_t cc_port) {
    SensorConfig::cc_port = cc_port;
}

int SensorConfig::getHeat_limit() const {
    return heat_limit;
}

void SensorConfig::setHeat_limit(int heat_limit) {
    SensorConfig::heat_limit = heat_limit;
}

int SensorConfig::getSmoke_limit() const {
    return smoke_limit;
}

void SensorConfig::setSmoke_limit(int smoke_limit) {
    SensorConfig::smoke_limit = smoke_limit;
}

std::string SensorConfig::getCC_Addr() const {
    return cc_addr;
}

void SensorConfig::setCC_Addr(int cc_addr) {
    SensorConfig::cc_addr = cc_addr;
}


int SensorConfig::getInfrared_limit() const {
    return infrared_limit;
}

void SensorConfig::setInfrared_limit(int infrared_limit) {
    SensorConfig::infrared_limit = infrared_limit;
}



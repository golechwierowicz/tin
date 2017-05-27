#include <SensorConfig.h>
#include <cstdio>

//
// Created by igor on 08.05.17.
//
SensorConfig::SensorConfig(SensorType type, in_port_t cc_port, std::string cc_addr)
        : type(type), cc_port(cc_port), cc_addr(cc_addr) {}

SensorConfig::~SensorConfig() {}

in_port_t SensorConfig::getCc_port() const {
    return cc_port;
}

void SensorConfig::setCc_port(in_port_t cc_port) {
    SensorConfig::cc_port = cc_port;
}

SensorType SensorConfig::getType() const {
    return type;
}

uint32_t SensorConfig::getId() const {
    return id;
}

const std::string &SensorConfig::getCc_addr() const {
    return cc_addr;
}

void SensorConfig::setCc_addr(const std::string &cc_addr) {
    SensorConfig::cc_addr = cc_addr;
}

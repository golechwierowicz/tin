//
// Created by igor on 16.05.17.
//

#include <AddressInfo.h>

AddressInfo::AddressInfo(in_port_t port, char* ip) : port(port), ip(ip){

}

const in_port_t AddressInfo::getPort() const {
    return port;
}

void AddressInfo::setPort(in_port_t port) {
    this->port = port;
}

const char* AddressInfo::getIp() const {
    return ip;
}

void AddressInfo::setIp(const char*) {
    this->ip = ip;
}

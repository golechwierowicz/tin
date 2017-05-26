//
// Created by igor on 16.05.17.
//

#ifndef TIN_SENSORINFO_H
#define TIN_SENSORINFO_H

#include <arpa/inet.h>
class AddressInfo {
private:
    in_port_t port;
    const char* ip;
public:
    AddressInfo(in_port_t, const char*);
    const in_port_t getPort() const;
    void setPort(in_port_t port);
    const char* getIp() const;
    void setIp(const char* ip);
};


#endif //TIN_SENSORINFO_H

#ifndef TIN_FIREDEPTCONFIG_H
#define TIN_FIREDEPTCONFIG_H

#include <string>
#include <netinet/in.h>

class FireDeptConfig {
    in_port_t port;

    void setDefaults();
public:
    FireDeptConfig();
    FireDeptConfig(const std::string& filename);

    in_port_t getPort() const { return port; }
    void setPort(in_port_t value) { port = value; }
};


#endif //TIN_FIREDEPTCONFIG_H

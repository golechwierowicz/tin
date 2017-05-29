#include <ConfigReader.h>
#include <Logger.h>
#include "FireDeptConfig.h"


FireDeptConfig::FireDeptConfig() {
    setDefaults();
}


FireDeptConfig::FireDeptConfig(const std::string& filename) {
    setDefaults();
    try {
        ConfigReader reader(filename);
        port = (in_port_t) reader.read_integer("fire-dept.port");
    } catch(const std::exception& e) {
        logError() << e.what();
    }
}


void FireDeptConfig::setDefaults() {
    port = 4099;
}
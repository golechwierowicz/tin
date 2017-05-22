#include "errorhandling.h"

void raiseError(const char *message, const char *error) {
    std::stringstream ss;
    ss << "UdpConnection: "
       << message
       << " (" << error << ")";
    throw std::runtime_error(ss.str());
}

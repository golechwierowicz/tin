#ifndef TIN_ERRORS_H
#define TIN_ERRORS_H

#include <sstream>
#include <cstring>

void raiseError(const char *message, const char *error = strerror(errno));

#endif //TIN_ERRORS_H

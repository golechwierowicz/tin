#include <cstring>
#include "logging.h"

int main() {
    log("It works!");
    log_error(strerror(errno));
}

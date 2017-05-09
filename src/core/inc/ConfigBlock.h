#ifndef TIN_CONFIGBLOCK_H
#define TIN_CONFIGBLOCK_H
#include <cstdint>


class ConfigBlock {
private:
    typedef int32_t heat_value_t;
    typedef int32_t smoke_value_t;
    typedef int32_t infrared_value_t;

public:
    ConfigBlock(heat_value_t heat_limit, smoke_value_t smoke_limit, infrared_value_t infrared_limit);
};

#endif //TIN_CONFIGBLOCK_H

#ifndef _CONTROL_CENTER_H_
#define _CONTROL_CENTER_H_

#include "ConfigBlock.h"

class ControlCenter {
private:
    std::vector<long> sensors;
public:
    ControlCenter();
private:
    void update(ConfigBlock config_block);

};

#endif // _CONTROL_CENTER_H_

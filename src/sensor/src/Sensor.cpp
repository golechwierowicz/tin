#include <Serializer.h>
#include <ACKBlock.h>

class Sensor {
private:
    Serializer serializer;
    Config config;

public:
    Sensor(Serializer serializer): serializer(serializer);

    // communication central - sensor
    void send_ack_to_cc(ACKBlock ackBlock) {

    }
    void reload_config() {

    }
};
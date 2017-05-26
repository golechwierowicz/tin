#include <Sensor.h>
#include <Deserializer.h>
#include <CommonBlock.h>
#include <blocks/BlockReader.h>
#include <blocks/CntSensorConfigBlock.h>
#include <Logger.h>
#include <blocks/RequestConfigBlock.h>

Sensor::Sensor(Serializer serializer) : serializer(serializer) {
    // dummy method, will need to implement reading conf form file
    config = init_config();
    con_send.open_socket();
    con_recv.open_socket();
    // will need to change the address when multiple sensor come online, port can stay
    addrInfo = new AddressInfo(4049, LOCALHOST);
    init_recv_connection();
}

void Sensor::init_recv_connection() {
    con_recv.bind_port(addrInfo->getPort());
}

Sensor::~Sensor() {
    con_recv.close_socket();
    con_send.close_socket();
    delete addrInfo;
}
// to be changed, needs to read conf from file/info sent by CC/any kind of init conf
SensorConfig* Sensor::init_config() {
    // jak sie tu uzyje normalnego stringa zdefiniowanego jako static w udpconnection to wywala blad
    // w funkcji inet_pton "UdpConnection: inet_pton() failed (Success)"
    // wiec uzywam tego extern char* localhost
    // return new SensorConfig(5, 5, 5, DEFAULT_CC_PORT, UdpConnection::LOCALHOST);

    return new SensorConfig(DEFAULT_SENSOR_TYPE, DEFAULT_CC_PORT, LOCALHOST);\
}

void Sensor::create_request_block() {
    RequestConfigBlock configBlock(addrInfo->getPort());
    configBlock.serialize(serializer);
}

void Sensor::send_request_msg() {
    serializer.clear();
    create_request_block();
    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);

    try {
        auto addr = UdpConnection::getAddress(config->getCc_addr(), config->getCc_port());
        con_send.send_msg(buffer, size, addr);
    } catch(const std::runtime_error& e) {
        logError() << e.what();
    }
}

void Sensor::receive_cc_config_msg(uint8_t *buf, size_t bufSize) {
    size_t bytes_read;

    try {
        con_recv.receive(buf, bufSize, bytes_read);
    } catch (const std::runtime_error& e) {
        logError() << e.what();
        return;
    }

    logDebug() << "Received buffer size " << bytes_read;

    BlockReader reader(buf, bytes_read);

    for(AbstractBlock* block : reader.blocks) {
        if(block->type == bt_cnt_sensor_config) {
            auto configBlock = (CntSensorConfigBlock*) block;

            reload_config(configBlock->port_id);

            central_ips.clear();
            for(auto& cnt_ip : configBlock->central_ips) {
                central_ips.push_back(cnt_ip);
            }

            log() << "Message: " << configBlock->toString();
        }
    }
}


void Sensor::reload_config(in_port_t port) {
    config->setCc_port(port);
}

void Sensor::close_connection() {
    con_send.close_socket();
    con_recv.close_socket();
}

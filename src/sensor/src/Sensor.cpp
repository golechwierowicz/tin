#include <Sensor.h>
#include <Deserializer.h>
#include <blocks/BlockReader.h>
#include <blocks/CntSensorConfigBlock.h>
#include <Logger.h>
#include <blocks/RequestConfigBlock.h>

Sensor::Sensor(Serializer serializer) :
        serializer(serializer),
        port(4049), // TO BE CHANGED AND READ FROM CONFIG
        ip_address(UdpConnection::LOCALHOST),
        config(init_config()) {
    con_send.open_socket();
    con_recv.open_socket();

    init_recv_connection();
}

void Sensor::init_recv_connection() {
    con_recv.bind_port(port);
}

Sensor::~Sensor() {
    con_recv.close_socket();
    con_send.close_socket();
}

SensorConfig* Sensor::init_config() {
    // to be changed, needs to read conf from file/info sent by CC/any kind of init conf
    // dummy method, will need to implement reading conf form file
    return new SensorConfig(st_temp_sensor, 1, 4040, UdpConnection::LOCALHOST);
}

void Sensor::send_request_msg() {
    serializer.clear();
    RequestConfigBlock configBlock(port, config->getId());
    configBlock.serialize(serializer);

    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);

    try {
        log() << "Sending: " << configBlock.toString();
        auto addr = UdpConnection::get_address(config->getCc_addr(), config->getCc_port());
        con_send.send_msg(buffer, size, addr);
    } catch(const std::runtime_error& e) {
        logError() << e.what();
    }
}

bool Sensor::receive_cc_config_msg(uint8_t *buf, size_t bufSize) {
    size_t bytes_read;

    try {
        con_recv.receive(buf, bufSize, bytes_read);
    } catch (const std::runtime_error& e) {
        logError() << e.what();
        return false;
    }

    logDebug() << "Received buffer size " << bytes_read;

    BlockReader reader(buf, bytes_read);

    for(auto& block : reader.blocks) {
        if(block->type == BlockType::cnt_sensor_config) {
            auto configBlock = reinterpret_cast<CntSensorConfigBlock*>(block.get());

            reload_config(configBlock->port_id);

            central_ips.clear();
            for(auto& cnt_ip : configBlock->central_ips) {
                central_ips.push_back(cnt_ip);
            }

            log() << "Message: " << configBlock->toString();
        }
    }
    return true;
}


void Sensor::reload_config(in_port_t port) {
    config->setCc_port(port);
}

void Sensor::close_connection() {
    con_send.close_socket();
    con_recv.close_socket();
}

void Sensor::unset_connection_timeout() {
    con_recv.unset_connection_timeout();
}

void Sensor::set_connection_timeout(long int sec, long int microsec) {
    con_recv.set_connection_timeout(sec, microsec);
}

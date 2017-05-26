#include <Sensor.h>
#include <iostream>
#include <Deserializer.h>
#include <CommonBlock.h>
#include <blocks/BlockReader.h>
#include <blocks/CntSensorConfigBlock.h>
#include <Logger.h>

Sensor::Sensor(Serializer serializer) : serializer(serializer) {
    // dummy method, will need to implement reading conf form file
    config = init_config();
    con_send.open_socket();
    con_recv.open_socket();
    // will need to change the address when multiple sensor come online, port can stay
    addrInfo = new AddressInfo(4049, LOCALHOST);
    // there is a probem with con_recv - program says 'invalid argument'
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
    return new SensorConfig(5, 5, 5, DEFAULT_CC_PORT, UdpConnection::LOCALHOST);
}

void Sensor::create_request_block() {
    serializer
            .begin_block(8)
            .write<uint16_t>(addrInfo->getPort())
            .write<const std::string&>(addrInfo->getIp())
            .end_block();
}

void Sensor::send_test_msg() {
    serializer.clear();
    serializer.begin_block(1)
            .write<const std::string&>("serialized string")
            .write<const std::string&>("\nsent from sensor\n")
            .end_block();

    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);

    auto addr = UdpConnection::getAddress(config->getCC_Addr(), config->getCc_port());
    con_send.send_msg(buffer, size, addr);
}

void Sensor::send_request_msg() {
    // will need to put some guard locks in here - shared access with sensor data sent to local central
    serializer.clear();
    create_request_block();
    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);

    auto addr = UdpConnection::getAddress(config->getCC_Addr(), config->getCc_port());
    con_send.send_msg(buffer, size, addr);
}

void Sensor::receive_cc_config_msg() {
    uint8_t buf[Serializer::BUFFER_SIZE];
    struct sockaddr_in cli_name;
    socklen_t addrlen;

    addrlen = sizeof(cli_name);

    ssize_t bytes = recvfrom(con_recv.socket_fd, buf, Serializer::BUFFER_SIZE, 0, (struct sockaddr*)&cli_name, &addrlen);
    if (bytes < 0) {
        perror("receiving datagram packet");
    }

    logDebug() << "Received buffer size " << bytes;

    BlockReader reader(buf, bytes);

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

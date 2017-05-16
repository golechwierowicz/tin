#include <Sensor.h>
#include <iostream>
#include <Deserializer.h>
#include <Connection.h>
#include <CommonBlock.h>

Sensor::Sensor(Serializer serializer) {
    // dummy method, will need to implement reading conf form file
    config = init_config();
    this->serializer = serializer;
    con_send = new Connection();
    con_recv = new Connection();
    con_send->create_socket();
    con_recv->create_socket();
    // will need to change the address when multiple sensor come online, port can stay
    addrInfo = new AddressInfo(4049, LOCALHOST);
    // there is a probem with con_recv - program says 'invalid argument'
    init_recv_connection();
}

void Sensor::init_recv_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, Connection::LOCALHOST, &my_name.sin_addr);
    my_name.sin_port = htons(addrInfo->getPort());

    if (bind(con_recv->_socket, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("init_recv_connection: binding datagram socket");
    }
}

Sensor::~Sensor() {
    con_recv->close_socket();
    con_send->close_socket();
    delete con_send;
    delete con_recv;
    delete addrInfo;
}
// to be changed, needs to read conf from file/info sent by CC/any kind of init conf
SensorConfig* Sensor::init_config() {
    return new SensorConfig(5, 5, 5, DEFAULT_CC_PORT, Connection::LOCALHOST);
}

void Sensor::create_request_block() {
    serializer
            .begin_block(8)
            .write(addrInfo->getPort())
            .write(std::string(addrInfo->getIp()))
            .end_block();
}

void Sensor::send_test_msg() {
    serializer.begin_block(1)
            .write(std::string("serialized string"))
            .write(std::string("\nsent from sensor\n"))
            .end_block();

    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);
    con_send->send_data(buffer, size, config->getCc_port(), config->getCC_Addr());
}

void Sensor::send_request_msg() {
    // will need to put some guard locks in here - shared access with sensor data sent to local central
    serializer.clear();
    create_request_block();
    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);
    con_send->send_data(buffer, size, config->getCc_port(), config->getCC_Addr());
}

void Sensor::receive_cc_config_msg() {
    uint8_t buf[Serializer::BUFFER_SIZE];
    struct sockaddr_in cli_name;
    socklen_t addrlen;

    addrlen = sizeof(cli_name);

    if (recvfrom(con_recv->_socket, buf, Serializer::BUFFER_SIZE, 0, (struct sockaddr*)&cli_name, &addrlen) == -1) {
        perror("receiving datagram packet");
    }

    int size = sizeof(buf);
    Deserializer d(buf, size);
    int block_type = d.get_block_type();
    std::cout << "Got block type: " << block_type << std::endl;
    assert(block_type == CNT_SENSOR_CONFIG);

    in_port_t new_cc_port;
    int neighborhood_station_count = 0;
    std::string cc_ip;
    d.read(new_cc_port);
    d.read(cc_ip);
    d.read(neighborhood_station_count);
    central_ips.clear();
    for(uint16_t i = 0; i < neighborhood_station_count; i++) {
        std::string cnt_ip;
        d.read(cnt_ip);
        central_ips.push_back(cnt_ip);
    }
    reload_config(new_cc_port);

    std::cout << "Successfully updated sensor with the following values: " << std::endl;
    std::cout << "CC Port: " << new_cc_port << std::endl;
    std::cout << "Central IPs: " << std::endl;
    for(auto ip: central_ips)
        std::cout << ip << std::endl;
    std::cout << "Control center IP: " << cc_ip << std::endl;
}


void Sensor::reload_config(in_port_t port) {
    config->setCc_port(port);
}

void Sensor::close_connection() {
    con_send->close_socket();
    con_recv->close_socket();
}

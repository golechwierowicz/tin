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
    // there is a probem with con_recv - program says 'invalid argument'; probably has to do with
    //std::cout << con_send->_socket << " " << con_recv->_socket;
    _default_CC_port = 4048; // dummy
    init_recv_connection();
}

void Sensor::init_recv_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, Connection::LOCALHOST, &my_name.sin_addr);
    my_name.sin_port = htons(config->getCc_port());

    if (bind(con_recv->_socket, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("init_recv_connection: binding datagram socket");
    }
}

Sensor::~Sensor() {
    con_recv->close_socket();
    con_send->close_socket();
    delete con_send;
    delete con_recv;
}
// to be changed, needs to read conf from file/info sent by CC/any kind of init conf
SensorConfig* Sensor::init_config() {
    return new SensorConfig(5, 5, 5, 4049, Connection::LOCALHOST);
}

void Sensor::create_request_block() {
    serializer
            .begin_block(8)
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
    con_send->send_data(buffer, size, _default_CC_port, config->getCC_Addr());
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

    in_port_t new_sensor_port; // sensors port
    int cps_size = 0;
    std::string cc_ip; // cc ip
    d.read(new_sensor_port);
    d.read(cc_ip);
    d.read(cps_size);
    central_ips.clear();
    for(uint16_t i = 0; i < cps_size; i++) {
        std::string cnt_ip;
        d.read(cnt_ip);
        central_ips.push_back(cnt_ip);
    }
    // we are updating the port the sensor listens on - we'd better do that for central destination ports
    //_port = sensor_id;
    config->setCc_port(new_sensor_port);
    // but since it is what it is, for now we need to reinitialize the connection with the new port number
    // or the next update will fail
    // and somehow unbind the previous port
    // the only way to free a bound port is to kill the process using it..
    // we definitely need to change this i.e. update the destination port not the sensor reception port
    init_recv_connection();

    std::cout << "Successfully updated sensor with following values: " << std::endl;
    std::cout << "Sensor Port: " << new_sensor_port << std::endl;
    std::cout << "Central ips: " << std::endl;
    for(auto ip: central_ips)
        std::cout << ip << std::endl;
    std::cout << "Control center ip: " << cc_ip << std::endl;
}

// TODO
void Sensor::reload_config() {

}

void Sensor::close_connection() {
    con_send->close_socket();
    con_recv->close_socket();
}

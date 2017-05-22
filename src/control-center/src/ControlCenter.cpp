#include <ControlCenter.h>
#include <Deserializer.h>
#include <Serializer.h>
#include <Connection.h>
#include <CommonBlock.h>
#include <iostream>
#include <blocks/CntSensorConfigBlock.h>
#include "AddressInfo.h"

ControlCenter::ControlCenter(Serializer serializer) {
    _serializer = serializer;
    connection = new Connection();
    con_send = new Connection();
    connection->create_socket();
    con_send->create_socket();
    init_connection();
}

ControlCenter::~ControlCenter() {}

void ControlCenter::init_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, Connection::LOCALHOST, &my_name.sin_addr);
    my_name.sin_port = htons(ControlCenter::port);

    if (bind(connection->_socket, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("controlcenter, init_connection: binding datagram socket");
    }
}

void ControlCenter::broadcast_sensors() {
   for(auto sensor: _sensors) {
     send_config_sensor_msg(sensor->getPort(), sensor->getIp());
   }
}

void ControlCenter::read_sensors() {
    // dummy, read from conf here
    // or wait for config requests
    // currently not used - implemented waiting for sensor request to register them
    _sensors.push_back(new AddressInfo(4049, const_cast<char*>(Connection::LOCALHOST)));
}

void ControlCenter::create_sensor_config_block(
        std::vector<std::string> central_ips,
        in_port_t port_id,
        std::string cnt_ip) {
    CntSensorConfigBlock block(central_ips, port_id, cnt_ip);
    block.serialize(_serializer);
}

std::vector<std::string> ControlCenter::get_central_ips() { // another dummy method, this should be hardcoded or read from config
    std::vector<std::string> result; 
    result.push_back("192.168.1.2");
    return result;
}

void ControlCenter::send_config_sensor_msg(const in_port_t port, const char* addr) {
    _serializer.clear();
    std::vector<std::string> central_ips = get_central_ips();
    uint16_t size;
    // will have to actually change the CC port in advance for the update to make any sense
    create_sensor_config_block(central_ips, ControlCenter::port, Connection::LOCALHOST);
    uint8_t* buff = _serializer.get_buffer(size);
    connection->send_data(buff, size, port, addr); 
}

void ControlCenter::recv_sensor_request_msg() {
    uint8_t buf[512];
    struct sockaddr_in cli_name;
    socklen_t addrlen;

    addrlen = sizeof(cli_name);
    if (recvfrom(connection->_socket, buf, 512, 0, (struct sockaddr*)&cli_name, &addrlen) == -1) {
        perror("receiving datagram packet");
    }

    int size = sizeof(buf);
    Deserializer*d = new Deserializer(buf, size);
    d->next_block();
    int block_type = d->get_block_type();
    std::cout << "Got block type: " << block_type << std::endl;
    assert(block_type == REQUEST_CONFIG);
    std::cout << "received config request\n\n";

    in_port_t sensor_port;
    std::string sensor_ip;
    d->read(sensor_port);
    d->read(sensor_ip);
    send_config_sensor_msg(sensor_port, sensor_ip.c_str());

    AddressInfo dto(sensor_port, const_cast<char*>(sensor_ip.c_str()));
    update_sensor_list(dto);
    delete d;
}

void ControlCenter::recv_test_sensor_msg() {
    uint8_t buf[512];
    struct sockaddr_in cli_name;
    socklen_t addrlen;

    addrlen = sizeof(cli_name);
    if (recvfrom(connection->_socket, buf, 512, 0, (struct sockaddr*)&cli_name, &addrlen) == -1) {
        perror("receiving datagram packet");
    }

    Deserializer d(buf, sizeof(buf));
    d.next_block();
    std::string string_value_1;
    std::string string_value_2;
    d.read(string_value_1);
    d.read(string_value_2);
    std::cout << string_value_1 << " " << string_value_2;
}

void ControlCenter::close_connection() {
    connection->close_socket();
}

void ControlCenter::update_sensor_list(AddressInfo info) {
    bool contains = false;
    for (AddressInfo* sensor : _sensors) {
        if (sensor->getIp() == info.getIp() && std::string(sensor->getIp()) == std::string(info.getIp())) {
            break;
        }
    }
    if (contains == false) {
        _sensors.push_back(new AddressInfo(info.getPort(), const_cast<char*>(info.getIp())));
    }
}
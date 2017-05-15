#include <ControlCenter.h>
#include <Deserializer.h>
#include <Serializer.h>
#include <Connection.h>
#include <CommonBlock.h>
#include <iostream>
#include <map>

ControlCenter::ControlCenter(Serializer serializer) {
    _serializer = serializer;
    connection = new Connection();
    con_send = new Connection();
    connection->create_socket();
    con_send->create_socket();
    read_sensors();
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
     send_config_sensor_msg(sensor.first, sensor.second.c_str());       
   }
}

void ControlCenter::read_sensors() {
    _sensors[4049] = Connection::LOCALHOST; // dummy, read from conf here
}

void ControlCenter::create_sensor_config_block(std::vector<std::string> central_ips,
        in_port_t port_id,
        std::string cnt_ip) {

    _serializer.begin_block(CNT_SENSOR_CONFIG);
    _serializer.write(port_id)
        .write(cnt_ip);
    int cps_size = central_ips.size();
    _serializer.write(cps_size);
    for(auto central_ip : central_ips) {
        _serializer.write(central_ip);
    }
    _serializer.end_block();
}

std::vector<std::string> ControlCenter::get_central_ips() { // another dummy method, this should be hardcoded or read from config
    std::vector<std::string> result; 
    result.push_back("192.168.1.2");
    return result;
}

void ControlCenter::send_config_sensor_msg(const in_port_t port, const char* addr) {
    std::vector<std::string> central_ips = get_central_ips();
    uint16_t size;
    create_sensor_config_block(central_ips, ControlCenter::port, Connection::LOCALHOST);
    uint8_t* buff = _serializer.get_buffer(size);
    connection->send_data(buff, size, port, addr); 
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
    std::string string_value_1;
    std::string string_value_2;
    d.read(string_value_1);
    d.read(string_value_2);
    std::cout << string_value_1 << " " << string_value_2;
}

void ControlCenter::close_connection() {
    connection->close_socket();
}

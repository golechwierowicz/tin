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
    _port = 4049; // dummy
    init_recv_connection();
}

void Sensor::init_recv_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, Connection::LOCALHOST, &my_name.sin_addr);
    my_name.sin_port = htons(_port);

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
Config* Sensor::init_config() {
    return new Config(5, 5, 5, 4048, Connection::LOCALHOST);
}

void Sensor::send_test_msg() {
    serializer.begin_block(1)
            .write(std::string("serialized string"))
            .write(std::string("\nsent\n"))
            .end_block();

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
    in_port_t sensor_id; // sensors port
    int cps_size = 0;
    std::string cc_ip; // cc ip
    d.read(sensor_id);
    d.read(cc_ip);
    d.read(cps_size);
    int cnt_ips_size = size - sizeof(sensor_id) - sizeof(cc_ip);
    central_ips.clear();
    for(uint16_t i = 0; i < cps_size; i++) {
        std::string cnt_ip;
        d.read(cnt_ip);
        central_ips.push_back(cnt_ip);
    }
    _port = sensor_id;
    std::cout << "Successfully updated sensor with following values: " << std::endl;
    std::cout << "Sensor Port: " << sensor_id << std::endl; 
    std::cout << "Central ips: " << std::endl;
    for(auto ip: central_ips)
        std::cout << ip << std::endl;
    std::cout << "Control center ip: " << cc_ip << std::endl;
}

void Sensor::close_connection() {
    con_send->close_socket();
    con_recv->close_socket();
}

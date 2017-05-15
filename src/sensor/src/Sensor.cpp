#include <Sensor.h>
#include <iostream>
#include <Deserializer.h>
#include <iostream>

Sensor::Sensor(Serializer serializer) {
    // dummy method, will need to implement reading conf form file
    config = init_config();
    this->serializer = serializer;
    con_send = new Connection();
    con_recv = new Connection();
    con_send->create_socket();
    con_recv->create_socket();
    // init_send_connection();
    _port = 4049; // dummy
    init_recv_connection();
}

void Sensor::init_send_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &my_name.sin_addr);
    my_name.sin_port = htons(config->getCc_port());

    if (bind(con_send->_socket, (struct sockaddr*)&my_name, sizeof(my_name)) == -1) {
        perror("init_send_connection: binding datagram socket");
    }
}

void Sensor::init_recv_connection() {
    struct sockaddr_in my_name;

    my_name.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &my_name.sin_addr);
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
    return new Config(5, 5, 5, 4048, "127.0.0.1");
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

void Sensor::receive_cc_test_msg() {
    uint8_t buf[512];
    struct sockaddr_in cli_name;
    socklen_t addrlen;

    addrlen = sizeof(cli_name);

    if (recvfrom(con_recv->_socket, buf, 512, 0, (struct sockaddr*)&cli_name, &addrlen) == -1) {
        perror("receiving datagram packet");
    }

    Deserializer d(buf, sizeof(buf));
    std::string string_value_1;
    std::string string_value_2;
    d.read(string_value_1);
    d.read(string_value_2);
    std::cout << string_value_1 << " " << string_value_2;
}

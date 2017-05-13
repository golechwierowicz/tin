#include <Sensor.h>
#include <iostream>

Sensor::Sensor(Serializer serializer){
    // dummy method, will need to implement reading conf form file
    config = init_config();
    this->serializer = serializer;
}

Config* Sensor::init_config() {
    config = new Config(5, 5, 5, 9999);
}

// will need to extract connection setup later
void Sensor::send_test_msg() {
    int sockd;
    struct sockaddr_in srv_addr;

    /* create a socket */
    sockd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockd == -1)
    {
        perror("Socket creation error");
        exit(1);
    }
    serializer.begin_block(1)
            .write(std::string("serialized string"))
            .end_block();
    uint16_t size;
    uint8_t* buffer = serializer.get_buffer(size);

    /* server address */
    srv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &srv_addr.sin_addr);
    srv_addr.sin_port = htons(10000);
    sendto(sockd, buffer, size, 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr));

    close(sockd);
    exit(0);
}
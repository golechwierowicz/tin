#include <Sensor.h>

Sensor::Sensor(Serializer serializer) {
    // dummy method, will need to implement reading conf form file
    config = init_config();
}

Config* Sensor::init_config() {
    config = new Config(5, 5, 5, 9999);
}

// will need to extract connection setup later
void Sensor::send_test_msg() {
    int sockd;
    struct sockaddr_in srv_addr;
    char buf[MAX_BUF];

    /* create a socket */
    sockd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockd == -1)
    {
        perror("Socket creation error");
        exit(1);
    }

    strcpy(buf, "Hello world\n");

    /* server address */
    srv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &srv_addr.sin_addr);
    srv_addr.sin_port = htons(10000);

    sendto(sockd, buf, strlen(buf)+1, 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    close(sockd);
    exit(0);
}
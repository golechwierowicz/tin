#include <ACKBlock.h>
#include <Sensor.h>

#include <sys/socket.h>
#include <cstdio>
#include <netdb.h>
#include <cstdlib>
#include <zconf.h>
#include <cstring>
#include <Serializer.h>
#include <Config.h>
#include <string>

Sensor::Sensor(Serializer serializer) {
    // dummy method, will need to implement reading conf form file
    config = init_config();
}

Config* Sensor::init_config() {
    config = new Config(5, 5, 5, 9999);
}

void Sensor::receive_test_msg() {
    int sock;
    unsigned int length;
    struct sockaddr_in name;
    char buf[1024];
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("opening datagram socket");
        exit(1);
    }

    /* Create name with wildcards. */
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = 9999;
    if (bind(sock, (struct sockaddr *) &name, sizeof name) == -1) {
        perror("binding datagram socket");
        exit(1);
    }

    length = sizeof(name);
    if (getsockname(sock, (struct sockaddr *) &name, &length) == -1) {
        perror("getting socket name");
        exit(1);
    }
    printf("Socket port #%d\n", ntohs(name.sin_port));
    /* Read from the socket. */
    if (read(sock, buf, 1024) == -1) {
        perror("receiving datagram packet");
    }
    printf("-->%s\n", buf);
    close(sock);
    exit(0);
}

// will need to extract connection setup later
void Sensor::send_test_msg() {
    char msg[] = "Hello world!";
    int sock;
    struct sockaddr_in dest;
    struct hostent *hp;

    /* Create socket on which to send. */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("opening datagram socket");
        exit(1);
    }

    hp = gethostbyname("127.0.0.1");
    if (hp == (struct hostent*)0) {
        fprintf(stderr, "%s: unknown host\n", "127.0.0.1");
        exit(2);
    }
    memcpy((char *) &dest.sin_addr, (char *) hp->h_addr, hp->h_length);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(config->getCc_port());

    /* Send message. */
    if (sendto(sock, msg, sizeof msg, 0, (struct sockaddr *) &dest,sizeof dest) == -1)
        perror("sending datagram message");
    close(sock);
    exit(0);
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>

class connect {
private:
    int sock;
    int port;
    char *ip;
    struct sockaddr_in server;
}

int main(int argc, char *argv[])
{
    int sock;
    int port;
    char *ip;
    struct sockaddr_in server;
    if (argc != 4) {
        printf("Usage: %s <ip> <port> <command>\n", argv[0]);
        return 1;
    }
    ip = argv[1];
    port = atoi(argv[2]);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return 1;
    }
    if (send(sock, argv[3], strlen(argv[3]), 0) < 0) {
        perror("send");
        return 1;
    }
    close(sock);
    return 0;
}
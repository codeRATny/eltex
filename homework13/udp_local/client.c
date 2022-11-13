#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define FILENAME_SIZE 108
#define BUFF_SIZE 128

int sock;
char server_path[FILENAME_SIZE] = "/tmp/mysock";
char client_path[FILENAME_SIZE] = "/tmp/myclientsock";

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);
    unlink(client_path);
    exit(EXIT_SUCCESS);
}

void error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    close(sock);
    unlink(client_path);
    exit(EXIT_FAILURE);
}

void creat_socket() {
    struct sockaddr_un name;

    sock = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }
    
    unlink(client_path);
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, client_path, sizeof(name.sun_path));
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';

    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
        error("bind");
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hello, World(client)";
    struct sockaddr_un server;
    int msg_len;
    int server_size = sizeof(server);

    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, server_path, sizeof(server.sun_path));

    creat_socket();

    msg_len = sendto(sock, send_buff, strlen(send_buff), MSG_CONFIRM, (struct sockaddr*)&server, server_size);
    if (msg_len < 0) {
        error("sendto");
    }

    msg_len = recvfrom(sock, recv_buff, BUFF_SIZE, 0, (struct sockaddr*)&server, &server_size);
    if (msg_len < 0) {
        error("recvfrom");
    }

    if (msg_len > 0) {
        recv_buff[msg_len] = '\0';
    }

    printf("RECEIVED: %s\n", recv_buff);

    return 0;
}
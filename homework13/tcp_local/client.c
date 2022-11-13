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
    exit(EXIT_FAILURE);
}

void creat_socket() {
    struct sockaddr_un name;

    sock = socket(AF_LOCAL, SOCK_STREAM, 0);
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
    
    creat_socket();
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hello, World(client)";
    struct sockaddr_un server;
    int msg_len;

    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, server_path, sizeof(server.sun_path));

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        error("connect");
    }

    msg_len = send(sock, send_buff, strlen(send_buff), MSG_CONFIRM);
    if (msg_len < 0) {
        error("send");
    }

    msg_len = recv(sock, recv_buff, BUFF_SIZE, MSG_WAITALL);
    if (msg_len < 0) {
        error("recv");
    }

    if (msg_len > 0) {
        recv_buff[msg_len] = '\0';
    }

    printf("RECEIVED: %s\n", recv_buff);
    return 0;
}
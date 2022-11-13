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

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);
    exit(EXIT_SUCCESS);
}

void error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    close(sock);
    exit(EXIT_FAILURE);
}

void run_client() {
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hello, World(client)";
    struct sockaddr_in server;
    int msg_len;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = 7777;

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        error("connect");
    }

    if (send(sock, send_buff, strlen(send_buff), MSG_CONFIRM) < 0) {
        error("send");
    }

    msg_len = recv(sock, recv_buff, BUFF_SIZE, MSG_WAITALL);
    if (recv(sock, recv_buff, BUFF_SIZE, MSG_WAITALL) < 0) {
        error("recv");
    }

    recv_buff[msg_len] = '\0';

    printf("RECEIVED: %s\n", recv_buff);
}

int main() {
    signal(SIGINT, handle_shutdown);
    run_client();
    return 0;
}
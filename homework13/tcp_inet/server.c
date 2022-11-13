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
    exit(EXIT_FAILURE);
}

void creat_socket() {
    struct sockaddr_in name;
    size_t size;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        error("socket");
    }

    name.sin_family = AF_INET;
    name.sin_addr.s_addr = inet_addr("127.0.0.1");
    name.sin_port = 7777;

    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
        error("bind");
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hello, World(server)";
    int msg_len;
    int sockClient;
    creat_socket();
    listen(sock, 5);
    while (1) {
        sockClient = accept(sock, 0, 0);
        if (sockClient < 0) {
            error("accept");
        }
        msg_len = recv(sockClient, recv_buff, BUFF_SIZE, 0);
        if (msg_len < 0) {
            error("recv");
        }

        if (msg_len > 0) {
            recv_buff[msg_len] = '\0';
        }

        msg_len = send(sockClient, send_buff, strlen(send_buff), MSG_CONFIRM);
        if (msg_len < 0) {
            error("send");
        }

        printf("RECEIVED: %s\n", recv_buff);
        close(sockClient);
    }
    return 0;
}
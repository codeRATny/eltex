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
char filename[FILENAME_SIZE] = "/tmp/mysock";

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);
    unlink(filename);
    exit(EXIT_SUCCESS);
}

void error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    close(sock);
    exit(EXIT_FAILURE);
}

int readable_timeo(int fd, int sec) {
    fd_set rset;
    struct timeval timeout;
    FD_ZERO(&rset);
    FD_SET(fd, &rset);

    timeout.tv_sec = 20;
    timeout.tv_usec = 0;
    return (select(fd + 1, &rset, NULL, NULL, &timeout));
}

void creat_socket() {
    struct sockaddr_un name;

    sock = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sock < 0) {
        error("socket");
    }

    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, filename, sizeof(name.sun_path));
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';
    unlink(filename);
    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
        error("bind");
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hello, World(server)";
    struct sockaddr_un client;
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
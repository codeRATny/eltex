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
    struct sockaddr_in name;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }
    
    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) {
        error("setsockopt fail");
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
    struct sockaddr_in client;
    int msg_len;
    int client_size = sizeof(client);

    creat_socket();
    
    while (1) {
        if (readable_timeo(sock, 40) == 0) {
            printf("Socket timeout\n");
            close(sock);
            exit(EXIT_SUCCESS);
        } else {
            msg_len = recvfrom(sock, recv_buff, BUFF_SIZE, 0, (struct sockaddr*)&client, &client_size);
            if (msg_len < 0) {
                error("recvfrom");
            }

            if (msg_len > 0) {
                recv_buff[msg_len] = '\0';
            }

            msg_len = sendto(sock, send_buff, strlen(send_buff), MSG_CONFIRM, (struct sockaddr*)&client, client_size);
            if (msg_len < 0) {
                error("sendto");
            }

            printf("RECEIVED: %s\n", recv_buff);
        }
    }

    return 0;
}
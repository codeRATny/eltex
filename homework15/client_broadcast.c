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

int main() {
    signal(SIGINT, handle_shutdown);
    
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hello, World(client)";
    struct sockaddr_in server;
    int msg_len;
    int server_size = sizeof(server);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("255.255.255.255");
    server.sin_port = 7777;

    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) == -1) {
        error("setsockopt fail");
    }
    if (bind(sock, (struct sockaddr*)&server, server_size) < 0) {
        error("bind");
    }
    while (1) {
        msg_len = recvfrom(sock, recv_buff, BUFF_SIZE, 0, (struct sockaddr*)&server, &server_size);
        if (msg_len < 0) {
            error("recvfrom");
        }

        if (msg_len > 0) {
            recv_buff[msg_len] = '\0';
        }

        printf("RECEIVED: %s\n", recv_buff);
    }

    return 0;
}
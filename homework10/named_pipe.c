#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int fd;

void handle_shutdown(int sig) {
    close(fd);
    printf("\nFile closed\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_shutdown);
    char* myfifo = "myfifo";

    mkfifo(myfifo, 0666);
    char data[80];

    if (!strcmp(argv[1], "1")) {
        fd = open(myfifo, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "open error: %s\n", strerror(errno));
        }

        while (1) {
            read(fd, data, 80);
            printf("Received data: %s\n", data);
        }
    } else if (!strcmp(argv[1], "2")) {
        fd = open(myfifo, O_WRONLY);
        if (fd == -1) {
            fprintf(stderr, "open error: %s\n", strerror(errno));
        }

        while (1) {
            fgets(data, 80, stdin);
            write(fd, data, strlen(data) + 1);
        }
    }

    return 0;
}
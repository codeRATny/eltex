#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void read_file(int fd) {
    FILE* stream;
    char c = '1';
    stream = fdopen(fd, "r");
    while ((c = fgetc(stream)) != EOF)
        putchar(c);
    fclose(stream);
}

void write_file(int fd) {
    FILE* stream;
    stream = fdopen(fd, "w");
    fprintf(stream, "apchihba\n");
    fprintf(stream, "aboba\n");
    fclose(stream);
}

int main() {
    pid_t pid;
    int mypipe[2];
    if (pipe(mypipe)) {
        fprintf(stderr, "pipe err\n");
        return EXIT_FAILURE;
    }

    pid = fork();

    if (pid == (pid_t)0) {
        /* Child process */
        close(mypipe[1]);
        read_file(mypipe[0]);
        return EXIT_SUCCESS;
    } else if (pid < (pid_t)0) {
        fprintf(stderr, "fork err\n");
        return EXIT_FAILURE;
    } else {
        /* Parent process */
        close(mypipe[0]);
        write_file(mypipe[1]);
        return EXIT_SUCCESS;
    }
    return 0;
}
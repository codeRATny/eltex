#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#define COMM_SIZE 128

typedef unsigned char byte;

const char *qq[] = {
    "q\n",
    "quit\n",
    "exit\n"
};

int pipe_exc(char **commands, char ***args, int commands_c)
{
    byte i;
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "pipe err\n");
        return -1;
    }
    int status = 0;
    pid_t* pid = malloc(sizeof(pid_t) * (commands_c - 1));

    for (i = 0; i < commands_c; i++) {
        pid[i] = fork();
        if (!pid[i]) {
            dup2(fd[0], STDIN_FILENO);
            if (i != (commands_c - 1)) {
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);
            close(fd[1]);

            execvp(commands[i], args[i]);
            perror("execvp");
            exit(EXIT_SUCCESS);
        }
    }

    close(fd[0]);
    close(fd[1]);

    for (i = 0; i < commands_c; i++) {
        waitpid(pid[i], &status, WUNTRACED);
    }

    free(pid);
    
    return 0;
}


int main(int argc, char *argv[])
{
    char input[COMM_SIZE];
    char **commands = NULL;
    char ***args = NULL;
    char *istr;
    char sep[] = " ";
    byte flag = 0;
    byte i, j;
    int commands_c = 0;
    int args_c = 0;
    int args_c_c;

    system("clear");
    fprintf(stdout, "Welcome to my bash\n");
    fprintf(stdout, "Write q/quit/exit for close bash\n");

    while(strcmp(input, "q")) {
        fprintf(stdout, "$ ");
        fgets(input, 50, stdin);

        if (input[0] == '\n') {
            fprintf(stdin, "\n");
            continue;
        }

        for (byte j = 0; j < sizeof(qq) / sizeof(char*); j++)
            if (!strcmp(input, qq[j])){
                exit(EXIT_SUCCESS);
        }

        istr = strtok(input, sep);
        while(istr != NULL) {
            if (flag == 0) {
                commands = realloc(commands, sizeof(char*) * (commands_c + 1));
                commands[commands_c] = malloc((sizeof(char) * strlen(istr)) + 1);
                args = realloc(args, sizeof(char*) * (args_c + 1));
                args_c_c = 0;
                args[args_c] = realloc(args[args_c], sizeof(char*) * (args_c_c + 1));
                args[args_c][args_c_c] = malloc((sizeof(char) * strlen(istr)) + 1);
                if (istr[strlen(istr) - 1] == '\n') {
                    istr[strlen(istr) - 1] = '\0';
                    strcpy(commands[commands_c], istr);
                    strcpy(args[args_c][args_c_c], istr);
                    //printf("n c[%d] = %s, a[%d][%d] = %s\n", commands_c, commands[commands_c], args_c, args_c_c, args[args_c][args_c_c]);
                } else {
                    strcpy(commands[commands_c], istr);
                    strcpy(args[args_c][args_c_c], istr);
                    //printf("n1 c[%d] = %s, a[%d][%d] = %s\n", commands_c, commands[commands_c], args_c, args_c_c, args[args_c][args_c_c]);
                }
                args_c++;
                args_c_c++;
                commands_c++;
                flag = 1;
            } else {
                if (!strcmp(istr, "|")) {
                    flag = 0;
                    args[args_c - 1] = realloc(args[args_c - 1], sizeof(char*) * (args_c_c + 1));
                    args[args_c - 1][args_c_c] = NULL;
                    args_c_c++;
                    //printf("arg[%d][%d] = %s\n", args_c, args_c_c, args[args_c - 1][args_c_c - 1]);
                    istr = strtok(NULL, sep);
                    continue;
                }
                if (istr[strlen(istr) - 1] == '\n') {
                    istr[strlen(istr) - 1] = '\0';
                    args[args_c - 1] = realloc(args[args_c - 1], sizeof(char*) * (args_c_c + 1));
                    args[args_c - 1][args_c_c] = malloc((sizeof(char) * strlen(istr)) + 1);
                    strcpy(args[args_c - 1][args_c_c], istr);
                    args_c_c++;
                    //printf("arg[%d][%d] = %s\n", args_c - 1, args_c_c - 1, args[args_c - 1][args_c_c - 1]);
                    args[args_c - 1] = realloc(args[args_c - 1], sizeof(char*) * (args_c_c + 1));
                    args[args_c - 1][args_c_c] = NULL;
                    args_c_c++;
                    //printf("arg[%d][%d] = %s\n", args_c - 1, args_c_c - 1, args[args_c - 1][args_c_c - 1]);
                } else {
                    args[args_c - 1] = realloc(args[args_c - 1], sizeof(char*) * (args_c_c + 1));
                    args[args_c - 1][args_c_c] = malloc((sizeof(char) * strlen(istr)) + 1);
                    strcpy(args[args_c - 1][args_c_c], istr);
                    args_c_c++;
                    //printf("arg[%d][%d] = %s\n", args_c - 1, args_c_c - 1, args[args_c - 1][args_c_c - 1]);
                }
            }
            istr = strtok(NULL, sep);
        }

        //printf("com_c = %d, arg_c = %d\n", commands_c, args_c_c);
        pipe_exc(commands, args, commands_c);

        flag = 0;
        for (i = 0; i < 1; i++) {
            free(commands[i]);
            commands[i] = NULL;
        }

        for (i = 0; i < args_c; i++) {
            for (j = 0; j < args_c_c; j++) {
                free(args[i][j]);
                args[i][j] = NULL;
            }
        }

        commands_c = 0;
        args_c = 0;
    }

    for (i = 0; i < 1; i++) {
        free(commands[i]);
        commands[i] = NULL;
    }

    for (i = 0; i < args_c; i++) {
        for (j = 0; j < args_c_c; j++) {
            free(args[i][j]);
            args[i][j] = NULL;
        }
    }

    return 0;
}
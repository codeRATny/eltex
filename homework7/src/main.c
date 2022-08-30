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

void pipe_exc(char **commands, char ***args, int commands_c)
{
    byte i;
    int status;
    pid_t pid;
    pid_t ppid = fork();
    
    if (ppid == 0) {
        int fd[commands_c][2];
        for (int i = 0; i < commands_c; i++) {
            if (pipe(fd[i]) < 0) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        for (int i = 0; i < commands_c; i++) {
            pid = fork();
            if (pid == 0) {
                dup2(fd[i][STDIN_FILENO], STDIN_FILENO);
                close(fd[i][STDOUT_FILENO]);
                if (i + 1 != commands_c) {
                    dup2(fd[i + 1][STDOUT_FILENO], STDOUT_FILENO);
                    close(fd[i + 1][STDIN_FILENO]);
                }
                execvp(commands[i], args[i]);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            close(fd[i + 1][STDOUT_FILENO]);
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        exit(EXIT_FAILURE);
    } else {
        int sp;
        do {
            waitpid(ppid, &sp, WUNTRACED);
        } while (!WIFEXITED(sp) && !WIFSIGNALED(sp));
    }
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

    for(;;) {
        fprintf(stdout, "$ ");
        fgets(input, 50, stdin);

        if (input[0] == '\n') {
            fprintf(stdin, "\n");
            continue;
        }

        for (int j = 0; j < (int)sizeof(qq) / sizeof(char*); j++) {
            if (strcmp(input, qq[j]) == 0) {
                exit(EXIT_SUCCESS);
            }
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
                    args_c_c++;
                    args[args_c] = realloc(args[args_c], sizeof(char*) * (args_c_c + 1));
                    args[args_c][args_c_c] = NULL;
                    //printf("arg[%d][%d] = %s\n", args_c, args_c_c, args[args_c][args_c_c]);
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
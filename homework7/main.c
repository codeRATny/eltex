#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char input[50];
    char **commands;
    char ***args;
    char *istr;
    char sep[] = " ";
    char flag = 0;

    int i, j;
    system("clear");
    int commands_c = 0;
    int args_c = 0;
    int args_c_c;
    while(strcmp(input, "q")) {
        printf("$ ");
        fgets(input, 50, stdin);
        istr = strtok(input, sep);
        while(istr != NULL) {
            if (flag == 0) {
                commands = realloc(commands, sizeof(char*) * (commands_c + 1));
                commands[commands_c] = malloc((sizeof(char) * strlen(istr)) + 1);
                commands_c++;
                args_c_c = 0;
            } else {
                if (!strcmp(istr, "|")) {
                    flag = 0;
                    strcpy(commands[commands_c - 1], istr);
                    istr = strtok(NULL, sep);
                    continue;
                }
                if (args_c * sizeof(char*) != sizeof(args)) {
                    args = realloc(args, sizeof(char*) * (args_c + 1));
                    args_c++;
                }
                args[args_c - 1] = realloc(args[args_c - 1], sizeof(char*) * (args_c_c + 1));
                args[args_c - 1][args_c_c] = malloc((sizeof(char) * strlen(istr)) + 1);
            }
            if (istr[strlen(istr) - 1] == '\n') {
                istr[strlen(istr) - 1] = '\0';
            }
            if (flag == 0) {
                strcpy(commands[commands_c - 1], istr);
                printf("command[%d] = %s\n",commands_c, commands[commands_c - 1]);
                flag = 1;
            } else {
                strcpy(args[args_c - 1][args_c_c], istr);
                printf("arg[%d] = %s\n",args_c_c, args[args_c - 1][args_c_c]);
                args_c_c++;
            }
            istr = strtok(NULL, sep);
        }

        flag = 0;
        for (i = 0; i < 1; i++) {
            free(commands[i]);
        }

        for (i = 0; i < args_c; i++) {
            for (j = 0; j < args_c_c; j++) {
                free(args[i][j]);
            }
        }

        commands_c = 0;
        args_c = 0;
    }

    // printf("c = %d\n", args_c);
    // printf("c_c = %d\n", args_c_c);
    // printf("%s\n", args[0][0]);
    // printf("%s\n", args[0][1]);
    // printf("%s\n", args[0][2]);

    return 0;
}
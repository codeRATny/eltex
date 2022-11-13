#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "write_to_file.h"

void write_to_file(char buffer[], char filename[])
{
    ssize_t ret;
    size_t text_length = strlen(buffer);
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if(fd == -1)
    {
        perror("open err");
        return;
    }
    
    ret = write(fd, buffer, text_length);
    if(ret == -1)
    {
        perror("write err");
        goto closeFile;
    } else if(ret < text_length)
    {
        printf("write err size\n");
        goto closeFile;
    }

closeFile:
    if(close(fd) == -1) {
        perror("close err");
    }

}
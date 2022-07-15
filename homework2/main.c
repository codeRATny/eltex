#include <stdio.h>

int main(void)
{
    int number = 270533154;
    char* ptr = (char*)&number;
    printf("X: %x\n", number);
    printf("D: %d\n", number);
    ptr += 2;
    *ptr = 1;
    printf("X: %x\n", number);
    printf("D: %d\n", number);
    return 0;
}    
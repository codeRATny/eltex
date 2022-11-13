#include "functional.h"
#include <stdio.h>

int main()
{
    int first = 0, second = 0, op = 0, res = -1, div_flag = 0;
    char quit = ' ';
    printf("Enter q for exit\n");
    while(quit != 'q') {
        printf("First number: ");
        scanf(" %d", &first);
        printf("Second number: ");
        scanf(" %d", &second);
        printf("1. ADD\n2. SUB\n3. MUL\n4. DIV\n");
        scanf(" %d", &op);
        switch (op)
        {
        case 1:
            res = add(first, second);
            break;
        case 2:
            res = subtr(first, second);
            break;
        case 3:
            res = multipl(first, second);
            break;
        case 4:
            res = div(first, second);
            div_flag = 1;
            break;
        
        default:
            printf("err id operation\n");
            break;
        }
        if(res == -1 && div_flag)
        {
            printf("Division by zero\n");
        } else {
            printf("RESULT: %d\n", res);
        }
        div_flag = 0;
    }
    return 0;
}
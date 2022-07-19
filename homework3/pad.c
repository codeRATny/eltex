#include <stdio.h>

struct test1 {
    char a;
    int b;
};

struct test2 {
    char a;
    int b;
} __attribute__((packed));

void binary_int(unsigned int value)
{
    for (int i = sizeof(int) * 8-1; i >= 0; i--)
    {
        printf("%d", (value & (1 << i)) >> i );
        if(i % 8 == 0)
            printf(" ");
    }
    putc('\n', stdout);
}

void binary_char(unsigned char value)
{
    for (int i = sizeof(char) * 8-1; i >= 0; i--)
    {
        printf("%d", (value & (1 << i)) >> i );
        if(i % 8 == 0)
            printf(" ");
    }
    putc('\n', stdout);
}

int main(void)
{
    char str[10] = {'A', 0, 0, 0, 0, 'B', 0, 0, 0, 0};
    struct test1* test1 = (struct test1 *)str;
    struct test2* test2 = (struct test2 *)str;

    printf("size padded = %lld\n", sizeof(*test1));
    printf("size packed = %lld\n", sizeof(*test2));

    printf("a padded = %c\n", test1->a);
    printf("a packed = %c\n", test2->a);

    printf("b padded = %d\n", test1->b);
    printf("b packed = %d\n", test2->b);

    printf("binary padded a ");
    binary_char(test1->a);
    printf("binary packed a ");
    binary_char(test2->a);

    printf("binary padded b ");
    binary_int(test1->b);
    printf("binary packed b ");
    binary_int(test2->b);

    return 0;
}

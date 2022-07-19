#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

typedef unsigned char byte;

struct Clients {
    char name[30];
    char surname[30];
    char phone[12];
};

byte places[SIZE] = {[0 ... SIZE-1] = 0};
struct Clients clients[SIZE];

void print_menu()
{
    system("clear");
    printf("=============================\n");
    printf("Press 'a': Add abonent\n");
    printf("Press 'p': Print all abonents\n");
    printf("Press 'r': Remove abonent\n");
    printf("Press 's': Search abonent\n");
    printf("Press 'q': Quit\n");
    printf("=============================\n");
}


byte free_place()
{
    for (byte i = 0; i < SIZE; i++) {
        if (places[i] == 0)
            return i;
    }
    return 255;
}

int is_equal(char* str1, char* str2)
{
    int i;
    for(i = 0; str1[i] != '\0' || str2[i] != '\0'; i++)
    {
        if(str1[i] != str2[i])
            return 0;
    }
    if(str1[i] == str2[i]) // compare \0
        return 1;
    return 0;
}

byte add_client()
{
    print_menu();
    byte index = free_place();
    
    if(index == 255) {
        printf("dictionary is full!\n");
        return -1;
    }

    printf("Enter name -> ");
    scanf("%29s", clients[index].name);
    printf("Enter surname -> ");
    scanf("%29s", clients[index].surname);
    printf("Enter phone number -> ");
    scanf("%11s", clients[index].phone);

    places[index] = 1;
    printf("=============================\n");
    return 0;
}

void remove_client(byte ind)
{
    print_menu();
    if ((ind > SIZE - 1) || (ind > 255))
        printf("out of range\n");
    else
        places[ind] = 0;
    printf("=============================\n");
}

void search_client(char info[])
{
    print_menu();
    for (byte i = 0; i < SIZE; i++) {
        if (places[i] != 0) {
            if ((is_equal(info, clients[i].name)) || (is_equal(info, clients[i].surname)) || (is_equal(info, clients[i].phone))) {
                printf("Found client:\n");
                printf("User id: %d\n", i);
                printf("Name: %s\n", clients[i].name);
                printf("Surname: %s\n", clients[i].surname);
                printf("Phone number: %s\n", clients[i].phone);
            }
        }
    }
    printf("=============================\n");
}

void list_clients()
{
    print_menu();
    for (byte i = 0; i < SIZE; i++) {
        if (places[i] != 0) {
            printf("Client %d\n", i + 1);
            printf("Name: %s\n", clients[i].name);
            printf("Surname: %s\n", clients[i].surname);
            printf("Phone number: %s\n", clients[i].phone);
        }
    }
    printf("=============================\n");
}

int main(void)
{
    byte id;
    char c = 'r';
    char tag[30];
    print_menu();
    while(c != 'q')
    {
        scanf(" %c", &c);
        switch (c)
        {
            case 'a':
                add_client();
                break;
            case 'p':
                list_clients();
                break;
            case 'r':
                printf("Write client id -> ");
                scanf("%c", &id);
                remove_client(id);
                break;
            case 's':
                printf("Write Name/Surname/Phone number -> ");
                scanf("%s", tag);
                search_client(tag);
                break;
            case 'q':
                printf("Quit\n");
                break;
            default:
                printf("Unknown command\n");
                break;
        }
    }

    return 0;
}
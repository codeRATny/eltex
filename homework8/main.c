#include <stdio.h>
#include <pthread.h>
 
int a = 0;
 
void* func(void* arg)
{
    for(int i = 0; i < 1000000; i++) {
        a++;
    }
    
    return 0;
}
 
int main()
{
    pthread_t tid;
    int* status = NULL;

    for(int i = 0; i < 5; i++) {
        pthread_create(&tid, NULL, func, NULL);
    }
 
    for(int i = 0; i < 5; i++) {
        pthread_join(tid, (void**)&status);
    }

    printf("a=%d\n", a);

    return 0;
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker_thread(void *arg)
{
    while(1){
      printf("This is worker_thread()\n");
      sleep(2);
    }
    pthread_exit(NULL);
}

int main()
{
        pthread_t my_thread;
        int ret;

        printf("In main: creating thread\n");
        ret =  pthread_create(&my_thread, NULL, &worker_thread, NULL);
        if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
        }
        while(1){
          printf("lol\n");
          sleep(1);
        }
        pthread_exit(NULL);
}
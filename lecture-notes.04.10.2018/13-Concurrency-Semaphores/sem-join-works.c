#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "mythreads.h"

sem_t done;

void *
child(void *arg) {
    sleep(5);
    printf("child\n");
    Sem_post(&done);
    return NULL;
}

int
main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    Sem_init(&done, 0);
    Pthread_create(&p, NULL, child, NULL);
    Sem_wait(&done);

    printf("parent: end\n");
    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "mythreads.h"

sem_t s; // global, shared

void *
child(void *arg) {
    printf("child\n");
    // something here
    return NULL;
}

int
main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    // init here
    Pthread_create(&p, NULL, child, NULL);
    // something here
    printf("parent: end\n");
    return 0;
}


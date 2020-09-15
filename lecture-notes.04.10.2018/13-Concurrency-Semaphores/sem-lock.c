#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mythreads.h"

#define PMAX (100)

volatile static int counter = 0;

sem_t lock;

void *
worker(void *arg) {
    int i;
    // something here: provide mutual exclusion
    for (i = 0; i < 1e6; i++) 
	counter++;
    // something here: end mutex
    return NULL;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "usage: sem-lock <numthreads>\n");
	exit(1);
    }
    int threads = atoi(argv[1]);
    pthread_t pid[PMAX];
    Sem_init(&lock, ????); // what value to init here?
    int i;
    for (i = 0; i < threads; i++) 
	Pthread_create(&pid[i], NULL, worker, NULL); 
    for (i = 0; i < threads; i++) 
	Pthread_join(pid[i], NULL); 

    printf("counter: %d\n", counter);

    return 0;
}


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
    Sem_wait(&lock);
    for (i = 0; i < 1e6; i++) 
	counter++;
    Sem_post(&lock);
    return NULL;
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
	fprintf(stderr, "usage: sem-lock <numthreads>\n");
	exit(1);
    }
    int threads = atoi(argv[1]);
    if (threads > PMAX) {
	fprintf(stderr, "%d threads is the max\n", PMAX);
	exit(1);
    }

    pthread_t pid[PMAX];

    Sem_init(&lock, 1);

    printf("begin\n");

    int i;
    for (i = 0; i < threads; i++) 
	Pthread_create(&pid[i], NULL, worker, NULL); 

    for (i = 0; i < threads; i++) 
	Pthread_join(pid[i], NULL); 

    printf("counter: %d\n", counter);

    return 0;
}


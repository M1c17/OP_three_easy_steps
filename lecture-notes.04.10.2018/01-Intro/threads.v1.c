#include <stdio.h>
#include <stdlib.h>
#include "common.h"

pthread_mutex_t m;
volatile int counter = 0; 
int loops;

void *worker(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
    // while a thread is executing a critical section
    // no other thread can execute the same critical section
	Pthread_mutex_lock(&m);
	counter++;
	Pthread_mutex_unlock(&m);
    }
    pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
    if (argc != 2) { 
	fprintf(stderr, "usage: threads <value>\n"); 
	exit(1); 
    } 
    loops = atoi(argv[1]);
    pthread_t p1, p2;
    printf("Initial value : %d\n", counter);
    Pthread_mutex_init(&m, NULL);
    Pthread_create(&p1, NULL, worker, NULL); 
    Pthread_create(&p2, NULL, worker, NULL);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("Final value   : %d\n", counter);
    return 0;
}


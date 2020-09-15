#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mythreads.h"

volatile int counter_1 = 0;
volatile int counter_2 = 0;

sem_t lock_1; // protect counter_1
sem_t lock_2; // protect counter_2

void *worker1(void *arg) {
    int i;
    for (i = 0; i < 1e6; i++) {
	Sem_wait(&lock_1); // grab lock_1 first
	Sem_wait(&lock_2);
	counter_1++;
	counter_2++;
	Sem_post(&lock_1);
	Sem_post(&lock_2);
    }
    return NULL;
}

void *worker2(void *arg) {
    int i;
    for (i = 0; i < 1e6; i++) {
	Sem_wait(&lock_2); // grab lock_2 first
	Sem_wait(&lock_1);
	counter_1++;
	counter_2++;
	Sem_post(&lock_2);
	Sem_post(&lock_1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t pid[2];
    Sem_init(&lock_1, 1);
    Sem_init(&lock_2, 1);
    Pthread_create(&pid[0], NULL, worker1, NULL); 
    Pthread_create(&pid[1], NULL, worker2, NULL); 
    Pthread_join(pid[0], NULL); 
    Pthread_join(pid[1], NULL); 
    printf("counter_1: %d\n", counter_1);
    printf("counter_2: %d\n", counter_2);
    return 0;
}


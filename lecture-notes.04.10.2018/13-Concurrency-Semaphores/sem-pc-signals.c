#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#include "mythreads.h"

int max;
int loops;
int *buffer;

int useptr  = 0;
int fillptr = 0;

sem_t empty;
sem_t full;

#define CMAX (10)
int consumers = 1;

void do_fill(int value) 
{
    buffer[fillptr] = value;
    fillptr = (fillptr + 1) % max;
}

int do_get()
{
    int tmp = buffer[useptr];
    useptr = (useptr + 1) % max;
    return tmp;
}

void *
producer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++) {
	Sem_wait(&empty);
	do_fill(i);
	Sem_post(&full);
    }

    // end case
    for (i = 0; i < consumers; i++) {
	Sem_wait(&empty);
	do_fill(-1);
	Sem_post(&full);
    }

    return NULL;
}
                                                                               
void *
consumer(void *arg)
{
    int tmp = 0;
    while (tmp != -1) {
	Sem_wait(&full);
	tmp = do_get();
	Sem_post(&empty);
	printf("%d\n", tmp);
    }
    return NULL;
}

int
main(int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "usage: %s <buffersize> <loops> <consumers>\n", argv[0]);
	exit(1);
    }
    max   = atoi(argv[1]);
    loops = atoi(argv[2]);
    consumers = atoi(argv[3]);
    assert(consumers <= CMAX);

    buffer = (int *) Malloc(max * sizeof(int));
    int i;
    for (i = 0; i < max; i++) {
	buffer[i] = 0;
    }

    Sem_init(&empty, max); // max are empty 
    Sem_init(&full, 0);    // 0 are full

    pthread_t pid, cid[CMAX];
    Pthread_create(&pid, NULL, producer, NULL); 
    for (i = 0; i < consumers; i++) {
	Pthread_create(&cid[i], NULL, consumer, (void *)i); 
    }
    Pthread_join(pid, NULL); 
    for (i = 0; i < consumers; i++) {
	Pthread_join(cid[i], NULL); 
    }
    return 0;
}

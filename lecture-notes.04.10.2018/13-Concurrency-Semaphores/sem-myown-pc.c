#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#include "mythreads.h"
#include "zemaphore.h"

int max;
int loops;
int *buffer;

int use  = 0;
int fill = 0;

Zem_t empty;
Zem_t full;
Zem_t mutex;

#define CMAX (10)
int consumers = 1;

void do_fill(int value) 
{
    buffer[fill] = value;
    fill++;
    if (fill == max)
	fill = 0;
}

int do_get()
{
    int tmp = buffer[use];
    use++;
    if (use == max)
	use = 0;
    return tmp;
}

void *
producer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++) {
	Zem_wait(&empty);
	Zem_wait(&mutex);
	do_fill(i);
	Zem_post(&mutex);
	Zem_post(&full);
    }

    // end case
    for (i = 0; i < consumers; i++) {
	Zem_wait(&empty);
	Zem_wait(&mutex);
	do_fill(-1);
	Zem_post(&mutex);
	Zem_post(&full);
    }

    return NULL;
}
                                                                               
void *
consumer(void *arg)
{
    int tmp = 0;
    while (tmp != -1) {
	Zem_wait(&full);
	Zem_wait(&mutex);
	tmp = do_get();
	Zem_post(&mutex);
	Zem_post(&empty);
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

    Zem_init(&empty, max);
    Zem_init(&full, 0); 
    Zem_init(&mutex, 1);

    pthread_t pid, cid[CMAX];
    Pthread_create(&pid, NULL, producer, NULL); 
    for (i = 0; i < consumers; i++) {
	Pthread_create(&cid[i], NULL, consumer, NULL); 
    }
    Pthread_join(pid, NULL); 
    for (i = 0; i < consumers; i++) {
	Pthread_join(cid[i], NULL); 
    }
    return 0;
}


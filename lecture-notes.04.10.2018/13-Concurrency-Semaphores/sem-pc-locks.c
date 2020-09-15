#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#include "mythreads.h"

int max;
int loops;
int *buffer;

int use  = 0;
int fill = 0;

sem_t empty;
sem_t full;
sem_t mutex;

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
	Sem_wait(&mutex);
	Sem_wait(&empty);
	do_fill(i);
	Sem_post(&full);
	Sem_post(&mutex);
    }

    // end case
    for (i = 0; i < consumers; i++) {
	Sem_wait(&mutex);
	Sem_wait(&empty);
	do_fill(-1);
	Sem_post(&full);
	Sem_post(&mutex);
    }

    return NULL;
}
                                                                               
void *
consumer(void *arg)
{
    int tmp = 0;
    while (tmp != -1) {
	Sem_wait(&mutex);
	Sem_wait(&full);
	tmp = do_get();
	Sem_post(&empty);
	Sem_post(&mutex);
	printf("%lld %d\n", (long long int)arg, tmp);
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
    Sem_init(&mutex, 1);   // mutex

    pthread_t pid, cid[CMAX];
    Pthread_create(&pid, NULL, producer, NULL); 
    for (i = 0; i < consumers; i++) {
	Pthread_create(&cid[i], NULL, consumer, (void *) (long long int)i); 
    }
    Pthread_join(pid, NULL); 
    for (i = 0; i < consumers; i++) {
	Pthread_join(cid[i], NULL); 
    }
    return 0;
}


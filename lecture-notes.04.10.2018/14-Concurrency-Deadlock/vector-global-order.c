#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mythreads.h"

#define PMAX (100)

#define VECTOR_SIZE (100)

typedef struct __vector {
    int   values[VECTOR_SIZE];
    sem_t lock;
} vector_t;

void vector_init(vector_t *v) {
    int i;
    for (i = 0; i < VECTOR_SIZE; i++) {
	v->values[i] = 0;
    }
    Sem_init(&v->lock, 1); 
}

void vector_add(vector_t *v_dst, vector_t *v_src) {
    if (v_dst < v_src) {
	Sem_wait(&v_dst->lock);
	Sem_wait(&v_src->lock);
    } else {
	Sem_wait(&v_src->lock);
	Sem_wait(&v_dst->lock);
    }
    int i;
    for (i = 0; i < VECTOR_SIZE; i++) {
	v_dst->values[i] = v_dst->values[i] + v_src->values[i];
    }
    Sem_post(&v_dst->lock);
    Sem_post(&v_src->lock);
}

vector_t v1;
vector_t v2;

void *
worker(void *arg) {
    long long int which_lock_first = (long long int) arg;
    int i;
    for (i = 0; i < 1e6; i++) {
	if (which_lock_first == 0) { 
	    vector_add(&v1, &v2);
	} else {
	    vector_add(&v2, &v1);
	}
    }
    return NULL;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "usage: sem <numthreads>\n");
	exit(1);
    }
    int threads = atoi(argv[1]);
    pthread_t pid[PMAX];
    vector_init(&v1);
    vector_init(&v2);
    int i;
    for (i = 0; i < threads; i++) 
	Pthread_create(&pid[i], NULL, worker, (void *) (long long int) i); 
    for (i = 0; i < threads; i++) 
	Pthread_join(pid[i], NULL); 

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mythreads.h"

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
  top:
    if (sem_trywait(&v_dst->lock) != 0) {
	//printf("retry 1\n");
	goto top;
    }
    if (sem_trywait(&v_src->lock) != 0) {
	//printf("retry 2\n");
	Sem_post(&v_dst->lock);
	goto top;
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

void *worker(void *arg) {
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
    pthread_t pid[2];
    vector_init(&v1);
    vector_init(&v2);
    Pthread_create(&pid[0], NULL, worker, (void *) 0); 
    Pthread_create(&pid[1], NULL, worker, (void *) 1); 
    Pthread_join(pid[0], NULL); 
    Pthread_join(pid[1], NULL); 
    return 0;
}


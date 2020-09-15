#ifndef __ZEMAPHORE_h__
#define __ZEMAPHORE_h__

#include "mythreads.h"

typedef struct __Zem_t {
    int value;
    pthread_cond_t  cond;
    pthread_mutex_t lock;
} Zem_t;

void Zem_init(Zem_t *z, int value) {
    z->value = value;
    Cond_init(&z->cond);
    Mutex_init(&z->lock);
}

void Zem_wait(Zem_t *z) {
    Mutex_lock(&z->lock);
    while (z->value <= 0)
	Cond_wait(&z->cond, &z->lock);
    z->value--;
    Mutex_unlock(&z->lock);
}

void Zem_post(Zem_t *z) {
    Mutex_lock(&z->lock);
    z->value++;
    Cond_signal(&z->cond);
    Mutex_unlock(&z->lock);
}


#endif // __ZEMAPHORE_h__

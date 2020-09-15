#ifndef __ZEMAPHORE_h__
#define __ZEMAPHORE_h__

typedef struct __Zem_t {
    int             value;
    pthread_cond_t  cond; // cond_signal(c), cond_wait(c, m)
    pthread_mutex_t lock; // mutex_lock(m),  mutex_unlock(m)
} Zem_t;

// can assume only called by one thread
void
Zem_init(Zem_t *z, int value) {
    z->value = value;
    // init lock and cond
}

void Zem_wait(Zem_t *z) {
    // use semaphore definition as your guide
}

void Zem_post(Zem_t *z) {
    // use semaphore definition as your guide
}

#endif // __ZEMAPHORE_h__

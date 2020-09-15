typedef struct _rwlock_t { 
    sem_t writelock; 
    sem_t lock; 
    int readers;
} rwlock_t;

void rwlock_init(rwlock_t *L) {
    L->readers = 0;
    sem_init(&L->lock, 1); 
    sem_init(&L->writelock, 1); 
}

void rwlock_acquire_readlock(rwlock_t *L) {
    sem_wait(&L->lock);                       // a1
    L->readers++;                             // a2
    if (L->readers == 1)                      // a3
	sem_wait(&L->writelock);              // a4
    sem_post(&L->lock);                       // a5
}

void rwlock_release_readlock(rwlock_t *L) {
    sem_wait(&L->lock);                       // r1
    L->readers--;                             // r2
    if (L->readers == 0)                      // r3
	sem_post(&L->writelock);              // r4
    sem_post(&L->lock);                       // r5
}

void rwlock_acquire_writelock(rwlock_t *L) { 
    sem_wait(&L->writelock);
}

void rwlock_release_writelock(rwlock_t *L) { 
    sem_post(&L->writelock);
}


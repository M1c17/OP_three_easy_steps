//
// SEMAPHORE: PSEUDO-CODE
//
sem_init(sem_t *s, int initvalue) {
    s->value = initvalue;
}

sem_wait(sem_t *s) { 
    while (s->value <= 0) 
        put_self_to_sleep(); // put self to sleep
    s->value--; 
}

sem_post(sem_t *s) { 
    s->value++; 
    wake_one_waiting_thread(); // if there is one
}

//
// IMPORTANT: each is done atomically
// (i.e., body of post() and wait() happen all at once)
// 



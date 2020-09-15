#ifndef __MYTHREADS_h__
#define __MYTHREADS_h__

#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <stdlib.h>
#include <semaphore.h>

void *Malloc(size_t size) {
    void *rc = malloc(size);
    assert(rc != NULL);
    return rc;
}

void
Pthread_mutex_lock(pthread_mutex_t *m)
{
    int rc = pthread_mutex_lock(m);
    assert(rc == 0);
}
                                                                                
void
Pthread_mutex_unlock(pthread_mutex_t *m)
{
    int rc = pthread_mutex_unlock(m);
    assert(rc == 0);
}
                                                                                
void
Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 	
	       void *(*start_routine)(void*), void *arg)
{
    int rc = pthread_create(thread, attr, start_routine, arg);
    assert(rc == 0);
}

void
Pthread_join(pthread_t thread, void **value_ptr)
{
    int rc = pthread_join(thread, value_ptr);
    assert(rc == 0);
}

void
Sem_init(sem_t *sem, unsigned int value) 
{
    int rc = sem_init(sem, 0, value);
    assert(rc == 0);
}

void Sem_wait(sem_t *sem) 
{
    int rc = sem_wait(sem);
    assert(rc == 0);
}

void Sem_post(sem_t *sem) 
{
    int rc = sem_post(sem);
    assert(rc == 0);
}


#endif // __MYTHREADS_h__

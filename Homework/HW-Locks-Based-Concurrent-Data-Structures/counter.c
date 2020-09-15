 #ifdef Linux
#define _GNU_SOURCE
#include <sched.h> // CPU_ZERO
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "threat_helper.h"

#define MILLION 1000000


typedef struct __counter_t {
    int             value;
    pthread_mutex_t lock;
    // I thing this is for padding with the thread
    //char pad[sizeof(pthread_mutex_t) - sizeof(int)];
}   counter_t;

void init (counter_t *c) {
    c->value = 0;
    Pthread_mutex_init(&c->lock, NULL);
}

void increment (counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement (counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value--;
    Pthread_mutex_unlock(&c->lock);
}
int get (counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

typedef struct __thread_info_t {
    counter_t *counter;
    pthread_t thread;
    int cpu_idx;
    // I thing this is for padding with the counter 
    //char pad[sizeof(counter_t *) - sizeof(int)];
}   thread_info_t;

static void *thread_function(void *arg) {
    thread_info_t *m = (thread_info_t *)arg;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(m->cpu_idx, &cpuset);
    Pthred_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    for (int i = 0; i < MILLION; i++)
        increment(m->counter);
    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char argv[]){
    // How many CPUs are available on the system you are using?
    int cpus = (int)sysconf(_SC_NPROCESSORS_ONLN), s;
    if (cpus == -1)
        handle_error_en(cpus, "sysconf");

    // cpus
    for (int i = 1; i <= cpus; i++) {
    // threads
        for (int l = 1; l <= cpus; l++) {
            thread_info_t *tinfo = malloc((size_t)l * sizeof(thread_info_t));
            if (tinfo == NULL)
                handle_error_en(errno, "malloc");
            struct timeval start, end;
            counter_t *counter = malloc(sizeof(counter_t));
            if (counter == NULL)
                handle_error_en(errno, "malloc");
            init(counter);

            s = gettimeofday(&start, NULL);
            if (s != 0)
                handle_error_en(s, "gettimeofday");
            for (int j = 0; j < l; j++) {
                tinfo[j].counter = counter;
                tinfo[j].cpu_idx = j % i;
                Pthread_create(&tinfo[j].thread, NULL, &thread_function, &tinfo[j]);
            }

                // Waiting for the created thread to terminate
            for (int k = 0; k < l; k++)
                Pthread_join(tinfo[k].thread, NULL);
            s = gettimeofday(&end, NULL);
            if (s != 0)
                handle_error_en(s, "gettimeofday");
            
            long long startusec, endusec;
            startusec = start.tv_sec * MILLION + start.tv_usec;
            endusec = end.tv_sec * MILLION + end.tv_usec;
            printf("%d cpus, %d threads\n", i, l);
            printf("global variable count: %d\n", get(counter));
            printf("Time (seconds): %f\n\n", 
                ((double)(endusec - startusec) / MILLION));
            Pthread_mutex_destroy(&counter->lock);
            free(counter);
            free(tinfo);
        }
    }
    return 0;
}
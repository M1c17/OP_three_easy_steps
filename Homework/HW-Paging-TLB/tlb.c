// compile => gcc -pthread tlb.c -o tlb in LINUX
/*Write the program, called tlb.c, that can roughly measure the cost of accessing each page.
 Inputs to the program should be: the number of pages to touch and the number of trials. */

//writing (non-)portable code
#define _GNU_SOURCE     // to work on gcc
#include <sched.h>      // CPU_ZERO
#include <unistd.h>     // sysconf
#include <stdlib.h>     // exit, malloc, free
#include <stdio.h>      // printf, fprintf
#include <pthread.h>    // pthread_self 
#include <errno.h>
#include <sys/times.h>  // times


void handle_error(int, const char *);

int main(int argc, char *argv[]){

    if (argc < 3){
        // Output to stderr guarantees that they show up
        // for programs I write where I know the output is immediately fed into another program
        fprintf(stderr, "Usage: tlb [#of pages] [#of trials]\n");
        exit(EXIT_FAILURE);
     }

    if (argc == 3){
        // set cpu affinity of a particular threat

        // Create a cpu_set_t object representing a set of CPUs
        cpu_set_t cpuset;
        // Clears set, so that it contains no CPUs.
        CPU_ZERO(&cpuset);
        // Add CPU cpu to set.
        CPU_SET(0, &cpuset);

        pthread_t current_thread = pthread_self();
        int rc = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);

        if (rc != 0){
            handle_error(rc, "pthread_setaffinity_np");
        }
    }
    // sysconf - get configuration inf at run time
    // _SC_PAGESIZE - Size of a page in bytes.
    long PAGESIZE = sysconf(_SC_PAGESIZE);
    // The number of clock ticks per second
    long clk_tck = sysconf(_SC_CLK_TCK);
    // given input
    int NUMPAGES = atoi(argv[1]);
    int trials = atoi(argv[2]);

    if (NUMPAGES <= 0){
        fprintf(stderr, "Invalid Numpages input\n");
        exit(EXIT_FAILURE);
    }
    // struct tms {
    //            clock_t tms_utime;  /* user time */
    //            clock_t tms_stime;  /* system time */
    //            clock_t tms_cutime; /* user time of children */
    //            clock_t tms_cstime; /* system time of children */
    //        };
    struct tms start_tms, end_tms;
    // malloc array
    char *a = malloc((size_t)NUMPAGES * (size_t)PAGESIZE);
    long jump = PAGESIZE / (long)sizeof(int);
    // for storing the processor time
    clock_t start, end;

    if ((int)(start = times(&start_tms)) == -1){
        handle_error(errno, "start time");
    }

    // accesing a large data structure => array 
    for (size_t j = 0; j < trials; j++){
        for (size_t i = 0; i < NUMPAGES * jump; i += jump){ 
            a[i] += 1;
        }
    }
    
    if ((int)(end = times(&end_tms)) == -1){
        handle_error(errno, "end time");
    }

    int n_loops = trials * NUMPAGES;

    // nanoseconds
     printf("%f %f %f\n", (double)(end - start) / (double)clk_tck * 1000000000 / n_loops,
         ((double)(end_tms.tms_utime - start_tms.tms_utime) / (double)clk_tck) *
             1000000000 / n_loops,
         ((double)(end_tms.tms_stime - start_tms.tms_stime) / (double)clk_tck) *
             1000000000 / n_loops);
  free(a);
  return 0;
}

void handle_error(int en, const char *msg){
    do {
        // set an error code errno
        errno = en;
        perror(msg);
        exit(EXIT_FAILURE);
    } while (0);
}
tlb.c
Displaying tlb.c.
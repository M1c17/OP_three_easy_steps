#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

/* Measure the costs of a system call */

int main(int argc, char *argv[]){

    int fd = open("./cost_sys_call.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    int n_iterations = 1000000;

    /* The tv argument is a struct timeval (as specified in <sys/time.h>):

           struct timeval {
               time_t      tv_sec;     /* seconds
               suseconds_t tv_usec;    /* microseconds
           };
    */
    struct timeval start, end;
    gettimeofday(&start, NULL);
    size_t i;
    for (i = 0; i < n_iterations; i++){
        read(fd, NULL, 0);
    }

    gettimeofday(&end, NULL);
    printf("system call: %f microseconds\n\n", (float) (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / n_iterations);
    close(fd);

    return 0;
}

// reapetedly call a simple system call and time how long it takes
// dividing the time by the number of iterations =  estimate of the cost of a system call
// precision and accuracy of timer ->gettimeofday()
// gettimeofday() => return the time in microseconds 
// this will tell you how many iterations of your 
// null system-call test you'll have to run in order to get a good measurement result
// is not precise enough for you, you might look into
// using the rdtsc instruction available on x86 machines



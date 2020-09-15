/* Not working.
sched_getaffinity() and the cpu_set_t structure that it uses are linux-specific,
and are not available on all platforms. Mac OSX in particular seems to be missing them.

I found a blog post from someone porting code to OSX who found an alternative using 
sysctlbyname(). The code on that site reimplements sched_getaffinity() on Mac OSX by
requesting the machdep.cpu.core_count, and using that to build their own version of cpu_set_t.

(http://www.hybridkernel.com/2015/01/18/binding_threads_to_cores_osx.html)
*/

// NOW IS WORKING !!!! YEIHHHH

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include "sched_getaffinity.c"


/* Measure the cost of a context switch */

int main(int argc, char *argv[]){
   cpu_set_t set;
   CPU_ZERO(&set);
   CPU_SET(0, &set);
   int n_iterations = 1000000;
   struct timeval start, end;

   int first_pipefd[2], second_pipefd[2];
   if (pipe(first_pipefd) == -1){
       // printf(stderr, "error first pipe\n");
       perror("pipe");
       exit(EXIT_FAILURE);
   }
   if (pipe(second_pipefd) == -1){
        // fprintf(stderr, "error second pipe\n");
        perror("pipe");
        exit(EXIT_FAILURE);
   }
   pid_t cpid = fork();

   if (cpid < 0){
        // fprintf(stderr, "failed fork\n");
        perror("fork");
        exit(EXIT_FAILURE);
   }
   else if (cpid == 0){
       // child process
        if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &set) == -1){
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < n_iterations; i++){
            read(first_pipefd[0], NULL, 0);
            write(second_pipefd[1], NULL, 0);
        }
   } else {
       // parent process
        if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &set) == -1){
            exit(EXIT_FAILURE);
        }
        gettimeofday(&start, NULL);
        for (size_t i = 0; i < n_iterations; i++){
            read(first_pipefd[0], NULL, 0);
            write(second_pipefd[1], NULL, 0);
        }
        gettimeofday(&end, NULL);
        printf("context switch: %f microseconds\n\n", (float) (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / n_iterations);
   }
    return 0;
}


// measuring the  cost of a context switch:
// pipe() => measuring the cost of communicating 
// ensure that your context-switching processes are located on the same processor
// most operating systems have calls to bind a process to a particular processor; on Linux, for example, the sched setaffinity() call
// By ensuring both processes are on the same processor, you are making sure to measure the cost of the OS stopping
// one process and restoring another on the same CPU.
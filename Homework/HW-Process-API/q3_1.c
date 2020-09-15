#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

    int rc = fork();
    if (rc < 0){
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0){
        // child (new process)
        printf("hello\n");
    }
    else{
        sleep(1);
        //int rc_wait = wait(NULL);
        // parent goes down this path (main)
        printf("goodbye\n");
    }
    return 0;
}

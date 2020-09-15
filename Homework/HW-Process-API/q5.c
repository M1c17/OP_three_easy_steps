#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int rc = fork();

    if (rc < 0){
        fprintf(stderr, "failed fork\n");
        exit(1);
    }
    else if (rc == 0){
        pid_t rc_wait = wait(NULL);
        printf("hello, I am child (rc_wait: %d), (pid:%d)\n",  rc_wait, (int) getpid());
    }
    else{
        pid_t rc_wait = wait(NULL);
        printf("hello, I am parent of %d, (rc_wait: %d), (pid:%d)\n", rc, rc_wait, (int) getpid());
    }
    return 0;
}
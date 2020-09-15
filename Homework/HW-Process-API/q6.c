#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int wstatus;
    pid_t rc = fork();

    if (rc < 0){
        fprintf(stderr, "failed fork\n");
        exit(1);
    }
    else if (rc == 0){
        pid_t rc_waitip = waitpid(-1, NULL, WUNTRACED | WCONTINUED);
        printf("hello, I am child (rc_waitip: %d), (pid:%d)\n",  rc_waitip, (int) getpid());
    }
    else{
        pid_t rc_waitip = waitpid(rc, &wstatus, WUNTRACED | WCONTINUED);
        printf("hello, I am parent of %d, (rc_waitip: %d), (pid:%d)\n", rc, rc_waitip, (int) getpid());
        printf("if the child terminated normally: %s\n", WIFEXITED(wstatus) ? "true" : "false");
    }
    return 0;
}
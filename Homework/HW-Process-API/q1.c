#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    int x = 100;
    int rc = fork();

    if (rc < 0){
        // forl failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0){
        printf("Child process\n");
        printf("-------------------\n");
        printf("old value of x: %d\n", x);
        x = 101;
        printf("new value of x: %d\n", x);
    }
    else{
        printf(" Parent process\n");
        printf("-------------------\n");
        printf("old value of x: %d\n", x);
        x = 102;
        printf("new value of x: %d\n", x);
    }
    return 0;
}
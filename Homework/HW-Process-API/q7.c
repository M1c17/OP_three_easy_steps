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
        close(STDOUT_FILENO);
        printf("hello, I am a child\n");
    }
    else{
        wait(NULL);
        printf("hello, I am a paretn\n");
    }
    return 0;
}
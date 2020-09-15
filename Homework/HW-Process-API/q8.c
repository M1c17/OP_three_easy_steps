#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int fd[2];
    // fd[0] - read
    // fd[1] - write
    char buf;
    
    if (pipe(fd) == -1){
        // fprintf(stderr, "Pipe Failed");
        // return 1;
        // perror("pipe");
        // exit(EXIT_FAILURE);
    }
    pid_t rc[2];
    // fork the first child 
    rc[0] = fork();

    if (rc[0] < 0){
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (rc[0] == 0){
        // Usually close one of the ends on every process 
        close(fd[0]);               // close unused read
        char str[] = "I am the first child.";
        write(fd[1], str, strlen(str));
        close(fd[1]);                   // Reader will see EOF
        waitpid(rc[1], NULL, 0);
    }
    else{
        // fork the second child
        rc[1] = fork();
        if(rc[1] < 0){
            // fork failed
            fprintf(stderr, "fork failed\n");
            exit(EXIT_FAILURE);
        }
        else if(rc[1] == 0) {
            close(fd[1]);           // Close unused
            // read this string a byte at a time from the pipe
            while (read(fd[0], &buf, 1) > 0){
                write(STDOUT_FILENO, &buf, 1);
            }
            write(STDOUT_FILENO, "\n", 1);
            close(fd[0]);
        }
    }
    return 0;
}
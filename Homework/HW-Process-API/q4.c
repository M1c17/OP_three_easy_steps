#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char *argv[]){
    int rc = fork();

    if (rc < 0){
        // failed fork
        fprintf(stderr, "failed fork\n");
        exit(1);
    }
    else if (rc == 0){
        char * const args[] = {"ls", "-l", "-a", "-h", NULL};
        char * const envp[] = {NULL};

        // Uncomment only one of this 
        execl("/bin/ls", "ls", "-l", "-a", "-h", (char*) NULL);
        // execlp("ls", "ls", "-l", "-a", "-h", (char*) NULL);
        // execle("/bin/ls", "ls", "-l", "-a", "-h", NULL, envp);

        // execv("/bin/ls", args);
        // //execvp("/bin/ls", args);
        // execvp("ls", args);
        // //execvpe("ls", argv, envp);
    }
    else {
        wait(NULL);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
    int rc = fork();
    int fd = open("./q2.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    write(fd, "First line\n", strlen("First line\n"));

    if (rc < 0){
        // failed fork
        fprintf(stderr, "failed fork\n");
        exit(1);
    }
    else if (rc == 0){
        const char *child_msg = "child writes a line\n";
        write(fd, child_msg, strlen(child_msg));
        printf("file descriptor in child process %d\n", fd);
    }
    else{
        wait(NULL);
        const char *parent_msg = "parent writes a line\n";
        write(fd, parent_msg, strlen(parent_msg));
        printf("file descriptor in parent process %d\n", fd);
    }
    FILE *fp;
    fp = fopen("./q2.txt", "r");
    int ch;
    printf("file contents: \n");
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }
    printf("\n");
    fclose(fp);
    close(fd);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         // stat(), lseek(), open(), read(), close()
#include <sys/types.h>      // stat(), lseek(), open()
#include <sys/stat.h>       // stat(),  
#include <fcntl.h>          // open(), close()
#include <stdbool.h>
#include <string.h>

#define default_n 10
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE);} while(0)

int main(int argc, char *argv[]){

    char *filename = argv[2];
    //printf("%s\n", argv[2]);
    int n;
    struct stat sb;
    int lines = 0, sz, offset;
    int count = 0;

    if (argc < 2) {
        n = default_n;
        filename = "text.txt";

    } else if (argc == 3 && argv[1][0] == '-') {
        n = atoi(&argv[1][1]);
    } else {
        handle_error("Usage: ./tail -<offset> <filename>");
    }

    if (stat(filename, &sb) == -1) {
        handle_error("stat error");
    }

    int fd = open(filename, O_RDONLY, 0);
    if (fd < 0) {
        handle_error("cannot open file");
    }

    char buffer[sb.st_size];
    while ((sz = read(fd, buffer, sb.st_size))) {
        for (int i = 0; i < sz; i++){
            if (buffer[i] == '\n') {
                //printf("lines: %d\n", lines); 
                ++lines;
            }
        }   
    }
    //printf("lines: %d\n", lines);

    lseek(fd, 0, SEEK_SET);

    int tmp;
    while ((sz = read(fd, buffer, sb.st_size))) {
        for (int j = 0; j < sz; j++){
            if (lines >= n) {
                if (buffer[j] == '\n') {
                    --lines;    
                }
            } else {
                tmp = j;
                break; 
            }   
        }
    }
    //printf("%d\n", tmp);

    // resetting pointer to start of file
    lseek(fd, tmp, SEEK_SET);

    while ((sz = read(fd, buffer, sb.st_size))) {
        for (int j = 0; j < sz; j++) {
            printf("%c", buffer[j]);
        }   
    }
    close(fd);
}

    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define default_N 10
#define MAXLEN 100
#define MAXLINE 100
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while(0)

void tail(char *line, int n, char *filename) ;

int main(int argc, char* argv[]){
    int n;
    char *line;
    char *filename = argv[2];
    
    if (argc < 2){
        n = default_N;
    } else if (argc == 3 && argv[1][0] == '-') {
        n = atoi(&argv[1][1]);
    } else {
        handle_error("Usage error: ./tail -<offset> <filename>");
    }

    tail(line, n, filename);
}

void tail(char *line, int n, char *filename) {

    int count;
    size_t len = 0;
    int first_line = 0, last_line = 0;
    

    char *p;
    char *ptrarray[MAXLINE];
    char *buffer;
    char *buff_end;

    if (n < 1 || n > MAXLEN) {
        handle_error("n error");
    }
    
    for (int i = 0; i < MAXLINE; i++) {
        ptrarray[i] = NULL;
    }

    //allocate buffer
    if ((p = buffer = malloc(MAXLEN * MAXLINE)) == NULL){
        handle_error("cannot allocate buffer");
    }

    buff_end = buffer + MAXLINE * MAXLEN;

    FILE *fd = fopen(filename, "r");
    if (fd == NULL) {
        handle_error("cannot open file");
    }

    while (getline(&line, &len, fd) != -1) {
        if ((p + len + 1) >= buff_end) {
            p = buffer;
        }

        ptrarray[last_line] = p;
        strcpy(ptrarray[last_line], line);
        ++last_line;

        if (last_line > MAXLINE) {
            last_line = 0;
        }

        p += len + 1;
        ++count;
    }

    if (n > count){
        n = count;
    }

    first_line = last_line - n;

    if (first_line < 0) {
        first_line += MAXLEN;
    }

    for (int j = first_line; n-- > 0; j = (j + 1) % MAXLINE) {
        printf("%s\n", ptrarray[j]);
    }

    fclose(fd);
    free(line);
}
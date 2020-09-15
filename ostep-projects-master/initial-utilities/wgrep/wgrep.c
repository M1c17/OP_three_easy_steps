#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define GETLINE_MINSIZE 16
#define MAXLINE 1000

/*
wgrep: This tool looks for a file line by line trying to find a user-specified
search term in the line. if the line has the word within it, the line is printed
out, otherwise it is not.
*/

int get_line(char **, size_t *, FILE *);
char* str_str(char *, char *);


int main(int argc, char *argv[]){

    FILE *fp;
    char *line = NULL;
    size_t linecap = 0;


    if (argc <= 1){
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    if (argc == 2){
        fp = stdin;
    }

    if (argc >= 3 && (fp = fopen(argv[2], "r")) == NULL){
        printf("wgrep: cannot open file\n");
        exit(1);
    }
    while(get_line(&line, &linecap, fp) > 0){
        if (str_str(line, argv[1])){
            printf("%s", line);
        }
    }
    free(line);
    fclose(fp);
    return 0;
}

/************************************************************
 You can use libraries instead if you like
 #include <stdio.h>
 ssize_t getline(char **lineptr, size_t *n, FILE *stream);
 #include <string.h>
 char *strstr(const char *haystack, const char *needle);
*************************************************************/

int get_line(char **lineptr, size_t *n, FILE *fp){
    int ch;
    int i = 0;
    char free_on_err = 0;
    char *p;

    errno = 0;
    if (lineptr == NULL || n == NULL || fp == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (*lineptr == NULL) {
        *n = GETLINE_MINSIZE;
        *lineptr = (char *)malloc( sizeof(char) * (*n));
        if (*lineptr == NULL) {
            errno = ENOMEM;
            return -1;
        }
        free_on_err = 1;
    }

    for (i=0; ; i++) {
        ch = fgetc(fp);
        while (i >= (*n) - 2) {
            *n *= 2;
            p = realloc(*lineptr, sizeof(char) * (*n));
            if (p == NULL) {
                if (free_on_err)
                    free(*lineptr);
                errno = ENOMEM;
                return -1;
            }
            *lineptr = p;
        }
        if (ch == EOF) {
            if (i == 0) {
                if (free_on_err)
                    free(*lineptr);
                return -1;
            }
            (*lineptr)[i] = '\0';
            *n = i;
            return i;
        }

        if (ch == '\n') {
            (*lineptr)[i] = '\n';
            (*lineptr)[i+1] = '\0';
            *n = i+1;
            return i+1;
        }
        (*lineptr)[i] = (char)ch;
    }
}

char* str_str(char *s, char *sub_s){
    while (*s){
        char *begin = s;
        char *pattern = sub_s;

        // If first character match, check for whole string
        while (*s && *pattern && *s == *pattern){
            s++;
            pattern++;
        }
        // If complete sub string match, return starting address
        if (!*pattern){
            return begin;
        }
        s = begin + 1; 
    }
    return NULL;
}
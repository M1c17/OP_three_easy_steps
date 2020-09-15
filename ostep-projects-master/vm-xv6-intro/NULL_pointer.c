#include <stdio.h>     // fwrite, fprintf
#include <stdlib.h>    // exit, malloc

// create a program that dereference a Null pointer


int
main (int argc, char* argv[]) {

    int *p;
    p = NULL;
    printf("pointer address: %p\n", p);
    printf("value %d\n", *p);

    return 0;
}

// where are programs loaded
// understand where are the segments: heap, stack, code, ...
// what happen when the computer is trying to dereference Null pointer
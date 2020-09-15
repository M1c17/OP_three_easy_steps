#include <stdio.h>
#include <stdlib.h>

int main(){

    int *x = (int *) malloc(1 * sizeof(int));
    x = NULL;
    // deference 
    printf("%d\n", *x);

    return 0;
}
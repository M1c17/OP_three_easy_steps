#include <stdio.h>
#include <stdlib.h>

int main(){

    int *x = (int *)malloc(sizeof(int));
    x = 9;
    printf("%d\n", *x);
    return 0;
}
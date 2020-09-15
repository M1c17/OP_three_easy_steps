#include <stdio.h>
#include <stdlib.h>

int main(){
    int *data = (int *)malloc(100 * sizeof(int));
    data[100] = 0;
    
    printf("%d\n", data[0]);

    for (int i = 0; i < 1; i++){
        printf("%d\n", data[i]);
    }

    free(&data[1]);
    //free(data);
    return 0;
}
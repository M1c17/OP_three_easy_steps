#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void print(int *);

int main(int argc, char *argv[]){

    if (argc != 3){
        printf("Usage: memory-user <memory> <runtime>\n");
        exit(EXIT_FAILURE);
    }

    printf("pid: %d\n", getpid());

    int memory = atoi(argv[1]) * 1000000;
    int length = (int) (memory / sizeof(int));
    int runtime = atoi(argv[2]);
    int *arr = malloc(memory);
    clock_t begin = clock();
    double time_spent;

    for (int i = 0; i < length; i++){
        time_spent = (double)(clock() - begin ) / CLOCKS_PER_SEC;
        if (time_spent >= runtime){
            break;
        }
        arr[i] += 1;
    }
    //print(arr);
    free(arr);
    return 0;
}

// void print(int *arr){
//     int i;
//     for (i = 0; i < 10; i++){
//         printf("%d", arr[i]);
//     }
// }
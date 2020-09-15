#include <stdio.h>
#include <stdlib.h>

/* 
wcat: reads a file as specified by the user and prints its contents 
*/

int main(int argc, char *argv[]){
    FILE *fp;
    char buffer[BUFSIZ];

    for (size_t i = 1; i < argc; i++){
        if ((fp = fopen(argv[i], "r")) == NULL){
            printf("wcat: cannot open file\n");
            exit(1);
        }
        while (fgets(buffer, BUFSIZ, fp) != NULL){
            printf("%s", buffer);
        }
    }
    fclose(fp);
    return 0;
}
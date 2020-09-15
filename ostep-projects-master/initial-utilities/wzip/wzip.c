#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/* 
wzip: is a file compresion tool.
The type of compression used here is a simple form of compression called run-length
encoding (RLE). RLE is quite simple: when you encounter n characters of the same type
in a row, the compression tool (wzip) will turn that into the number n and a single 
instance of the character.

https://linux.die.net/man/3/htonl
Littleendian and Bigendian byte order illustrated
    https://dflund.se/~pi/endian.html
*/

void writeFile(int , char *);

int main(int argc, char *argv[]){

    FILE *fp;
    char newbuff[2], oldbuff[2];
    int count;

    if (argc < 2){
        printf("wzip: file1 [file2 ...]\n");
        exit(EXIT_FAILURE);
    }

    // open files
    for (size_t i = 1; i < argc; i++){
        if ((fp = fopen(argv[i], "r")) == NULL){
            printf("wzip: cannot open file\n");
            exit(EXIT_FAILURE);
        }

        while (fread(newbuff, 1, 1, fp)){
            if (strcmp(newbuff, oldbuff) == 0){
                count++;
            } else {
                if (oldbuff[0] != '\0'){
                    writeFile(count, oldbuff);
                }
                count = 1;
                strcpy(oldbuff, newbuff);
            }
        }
        fclose(fp);
    }
    writeFile(count, oldbuff);
    return 0;
}

void writeFile(int count, char *oldbuff){

    // write as network byte order
    count = htonl(count);
    fwrite(&count, 4, 1, stdout);
    fwrite(oldbuff, 1, 1, stdout);
}
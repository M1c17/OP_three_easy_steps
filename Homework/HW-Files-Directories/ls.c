#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>        // opendir(), readdir()
#include <sys/stat.h>
#include <unistd.h>        //  getcwd(), getopt()
#include <sys/types.h>     //  stat()
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>         // ctime()

#define MAX_PATH 256
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

void print_file(struct stat sb) {
    printf("%ld\n", (long) sb.st_ino);
    printf("%lo\n", (unsigned long) sb.st_mode);
    printf("%hu\n", sb.st_nlink);
    printf("UID=%ld        GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);
    printf("%lld\n", (long long) sb.st_size);
    printf("%lld\n", (long long) sb.st_blocks);
    printf("%s", ctime(&sb.st_ctime));
    printf("%s", ctime(&sb.st_atime));
    printf("%s", ctime(&sb.st_mtime));
}

void print_dir_names(char *pathfile){
    DIR* dirp;
    struct dirent* direntp;
    //printf("here\n");
    //printf("%s\n", pathfile);

    if ((dirp = opendir(pathfile)) == NULL) {
        handle_error("cannot open directory");
    }
    while ((direntp = readdir(dirp)) != NULL){
        printf("%s\n", direntp->d_name);
    }
    if (direntp == NULL){
         handle_error("cannot read directory");
    }
    closedir(dirp);
}


int main(int argc, char *argv[]) {

    int opt;
    char *pathfile = NULL;
    bool flagList = false;
    char cwd[MAX_PATH];
    struct stat sb;

    for (int i = 1; i < argc; i++) {
        char *option = argv[i];
        char tmp = argv[i][0];
        //printf("temp: %c\n", tmp);
        //printf("option: %s\n", option);

        switch (tmp)
        {
        case '-':
            if (strcmp(option, "-l") == 0){
                printf("here\n");
                flagList = true;
            }
            break;
        
        default:
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                handle_error("getcwd error");
            } else {
                pathfile = cwd;
                //printf("%s\n", pathfile);
            }
            break;
        }
    }

    if (argc < 3) {   
        print_dir_names(pathfile);
    } else {
        // printf("boolean %d\n", flagList);
        if (flagList){
            if (argc < 4){
                printf("without path: %s\n", pathfile);
                print_file(sb);
            } else {
                //printf("argv is: %s\n", argv[3]);
                if (strcmp(argv[3], "-l") == 0){
                    printf("ls with path: %s\n", pathfile);
                    print_dir_names(pathfile);
                }
                pathfile = argv[3];
                printf("with path: %s\n", pathfile);
                print_file(sb);
            }           
        } else {
            //printf("hey hey hey\n");
            pathfile = argv[2];
            print_dir_names(pathfile);
        }    
    }
    exit(EXIT_SUCCESS);
}
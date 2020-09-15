#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXSIZE 100
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while(0)

char** find_dir(char* pathfile){
    DIR *dirp;
    struct dirent* direntp;
    struct stat sb;
    //int i;
    //char **array = (char **)malloc(BUFSIZ* sizeof(char*));

    if ((dirp = opendir(pathfile)) == NULL) {
        handle_error("cannot open directory");
    }
    
    // for (int j = 0; j < MAXSIZE; j++){
    //     array[j] = NULL;
    // }

    if (stat(pathfile, &sb) == -1) {
        handle_error("stat error");
    }

    while ((direntp = readdir(dirp)) != NULL){
        // array[i] = direntp->d_name;
        // printf("%s\n", array[i]);  
        // i++;
        if (S_ISREG(sb.st_mode)) {
            printf("%s\n", direntp->d_name);
            // printf("%s\n", array[i]);
        } else {
            if (S_ISDIR(sb.st_mode)) {
                find_dir(pathfile);
            }
        }   
    }

    if (direntp == NULL){
        handle_error("cannot read directory");
    }
    closedir(dirp);
    return array;
}

int main(int argc, char* argv[]) {
    char *pathfile;
    char cwd[256];

    if (argc < 2) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            handle_error("getcwd error");
        } else {
            pathfile = cwd;
        //printf("%s\n", pathfile);
        }
    } else {
        pathfile = argv[1];
        //printf("main %s\n", pathfile);
    }

    find_dir(pathfile);   
}
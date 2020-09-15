/*
Author: Micaela Pictor
Email: pictor117@gmail.com
CS-537: Introduction to Operating Systems
Operating Systems: Three Easy Pieces
Date :June/09/2020

Description: Implementation of basic Unix command line interpreter (shell) called wish
             this project has the following functionalities:
             - Built-in-command
             - Support batch mode-> read input from the file & executes commands from therein
             - Support interactive mode-> allows user type commands directly
             - Redirections
             - Parallel commands
*/
  
#include <stdio.h>       // fopen, fclose, fileno, getline, feof
#include <stdlib.h>      // exit
#include <sys/types.h>
#include <unistd.h>      // fork, exec, access, exit, chdir, dup2
#include <sys/wait.h>    // waitpid
#include "wish.h"
#include <regex.h>       // regcomp, regexec, regfree
#include <pthread.h>     // pthread_create, pthread_join
#include <ctype.h>       // isspace


// Global variables
char *line = NULL;
FILE *inpFile = NULL;
// Track the number of arguments 
int numArgs = 0;
// Track the number of commands
int n_commands = 0;

// initial shell should contain one directory /bin
char *paths[BUFF_SIZE] = {"/bin", NULL};

char *trim(char *s);
void *parseInput(char **arg);
char **runParallel(char *line);
int new_Process(char *args[], FILE *outFile, char *path);
int builtIn(char *args[]);
int execute_command(char **args, FILE *outFile);
int search_path(char path[], char *firstArg);
int built_cd(char **args);
int built_path(char **args);
int built_exit(char **args);
int num_builtIn();
void redirection(FILE *outFile);

void clean(void);

//----------------------------
    // Main shell loop
    // Description: Perform the main operations of the shell and continue looping 
    // (get the command prompt, execute programs, etc) until user specifies otherwise
    // exit command
//----------------------------
int main(int argc, char *argv[])
{
    int mode = INTERACTIVE_MODE;
    inpFile = stdin;
    char *word = malloc(1 * sizeof(char *));
    FILE *outFile = stdout;

    if (argc > 1){
        mode = BATCH_MODE;
        char *batch_file = strdup(argv[1]);
        inpFile = fopen(batch_file, "r");
        if (argc > 2 || inpFile == NULL){
            printError();
            exit(EXIT_FAILURE);
        } 

    }
    size_t len_line = 0;
    ssize_t n_lines;
    //loop repeadly read, parse, execute
    do {

        if (mode == INTERACTIVE_MODE){
            print_prompt();
        }

        // get line and read line
        // ssize_t getline(char **lineptr, size_t *n, FILE *stream);
        if ((n_lines = getline(&line, &len_line, stdin)) > 0){
            // feof tests the end-of-file indicator for the given stream
            //printf("%s", line);

            // remove new line characters
            if (line[n_lines - 1] == '\n'){
                line[n_lines - 1] = '\0';
            }

            char **args = runParallel(line);
            printf("commands: %d\n", n_commands);
            
            char **myargs = malloc(n_commands * sizeof(char*));
            char *command;
            int position = 0;
            // parse input and execute command
            for (size_t i = 0; i < n_commands; i++){
                printf("here %s\n", args[i]);
                //parseInput(&args[i]);
                if (!args) {
                    printError();
                }
                command  = strtok(args[i], "\n\t ");
                while(command != NULL){
                    myargs[position] = trim(command);
                    position++;
                    //printf("command: %s\n", command);
                    command = strtok(NULL, "\n\t "); 
                    if (position >= BUFF_SIZE){
                        printError();
                        break;
                    }
                }
            }
            // numArgs bug !!!!!!! when run in parallel 
            myargs[position] = NULL;
            numArgs = position;
            printf("Args: %d\n",numArgs);
            // reset n_commands new prompt
            n_commands = 0;

            // redirect output 
            // for(size_t i = numArgs; i >= 0; i--){
            //     if(strcmp(command, ">") == 0){
            //         printf("hey 1111 %s\n", args[i - 1]);
            //         if ((outFile = fopen(args[i - 1], "w")) == NULL){
            //             printf("heyyyyy %s\n", args[i - 1]);
            //             printError();
            //         }cd
            //     }
            // }

            if (myargs > 0){
                execute_command(myargs, outFile);
            }
        }
        else if (feof(inpFile) != 0) {
                clean();
                exit(EXIT_SUCCESS);
        }

    } while (1);
    return 0;
}


//----------------------------
    // new_Process
    // Description: create process for each new command
//-----------------------------
int new_Process(char *args[], FILE *outFile, char *path){

    pid_t pid;
    pid = fork();
    
    // catch fork erros
    if (pid < 0){
        printError();
        exit(EXIT_FAILURE);
    }
    // Child process
    else if (pid == 0){

        redirection(outFile);

        //int execv(const char *path, char *const argv[]);
        if (execv(path, args) == -1){
            //printf("here is\n");
            printError();
        }
    }
    // Parent process wait for child process
    else{
        // wait for process to finish
        waitpid(pid, NULL, 0);
    }
    return 1;
}

// To keep track the built in command functions this program have
// we can change and add more in the future
char *builtInArray[] = {
    "cd",
    "path",
    "exit",
};

int (*buitin_func[])(char **) = {
    &built_cd,
    &built_path,
    &built_exit,
};

int num_builtIn(void){
    //printf("numBuiltIn: %lu\n",sizeof(builtInArray) / sizeof(char *));
    return sizeof(builtInArray) / sizeof(char *);
}

//----------------------------
    // builtsIn
    // Description: How does the shell know to run? -> shell path
    // BUILT-IN-COMMANDS:
    // implement exit -> exit(0)
    //           cd   -> one argument max
    //           path -> 0 or more arguments
//-----------------------------
int built_cd(char **args){
    if (numArgs == 1 || numArgs > 2){
        printf("here");
        printError();
    } 
    else {
        if (chdir(args[1]) == -1){
            printError();
        }
    }
    return 1;
}
int built_path(char **args){
    size_t i = 0;
    paths[0] = NULL;
    while(i < numArgs - 1){
        paths[i] = strdup(args[i+1]);
        i++;
    }
    paths[i+1] = NULL;
    return 1;
}

int built_exit(char **args){
    if (numArgs > 1){
        printError();
    } else {
        atexit(clean);
        exit(EXIT_SUCCESS);
    }
    return 0;
}

//----------------------------
    // execute_command
    // Description: Whenever your shell accepts a command, it should check whether the command is
    // a built-in command** or not.
    
//-----------------------------
int execute_command(char **args, FILE *outFile){

    char path[BUFF_SIZE];

    // check for empty command
    if (args[0] == NULL){
        //printf("empty command");
        return 1;
    }

    for (int i = 0; i < num_builtIn(); i++){

        // printf("argument: %s\n", args[0]);
        // printf("builtIn: %s\n", builtInArray[i]);
        // printf("%d\n", strcmp(args[0], builtInArray[i]));
        if (strcmp(args[0], builtInArray[i]) == 0){
            return (*buitin_func[i])(args);
        }
    }

    if (search_path(path, args[0]) == 0){
        return new_Process(args, outFile, path);
    }
    else {
        printError();
    }
    return -1;
}

//----------------------------
    // search_path
    // Description: how does the shell to run /bin/ls?
    // when user specify path variable -> search for executables
    // search path-> set of directories that comprise the path
    // path variable-> contains the list of all directories to search, in order. 

    // access()->to check if a particular file exists in a directory
    // int access(const char *pathname, int mode);

    // input : array->path & array pointer->firsargv
    // output: int-> 0 if found path & -1 if found error
//-----------------------------
int search_path(char path[], char *firstArg){
    int i = 0;
    // search executable file in path
    while (paths[i] != NULL){
        snprintf(path, BUFF_SIZE, "%s/%s", paths[i], firstArg);
        // found path
        if (access(path, X_OK) == 0){
            return 0;
        }
        i++;
    }
    printf("path: %s\n", path);
    //empty path the shell should not be able to run any program
    return -1;
}

//----------------------------
    // redirection
    // Description: output of a program to a file rather than to the screen
    // the standard error output of the program should be rerouted to the file output
    // if the output file exists before run your program, you should should simple overwrite it
    // redirection symbol -> '>'
    // format-> i.e.: wish> `ls -la /tmp > output`
    // fileno() examines the argument stream and returns its integer descriptor
//-----------------------------
void redirection(FILE *outFile){
    int outFileno;
    
    // something wrong happen no srgument stream
    if ((outFileno = fileno(outFile)) == -1){
        printError();
        return;
    }
    // no end of file
    if (outFileno != STDOUT_FILENO){
        // the standard output of the program should be rerouted to the file
        if (dup2(outFileno, STDOUT_FILENO) == -1){
            printError();
            return;
        }
        // the standard error output of the program should be rerouted to the file
        if (dup2(outFileno, STDERR_FILENO) == -1){
            printError();
            return;
        }
        fclose(outFile);
    }
}

//----------------------------
    // runParallel
    // Description: shell should run in parallel before waiting for any of them to complete.
    // Then, after starting all such procress you must wait for them to complete
    // After all done, return control to the user or if batch mode move on to the next line
    // wish> : cmd 1 & cmd 2 args 1 args 2 & cmd 3 args 1
//-----------------------------
char **runParallel(char *line){  
    // allocate myArgs array
    char **args = malloc(BUFF_SIZE * sizeof(char*));
    char *command;

    char *temp = line;
    
    if (!args) {
        //printf("here");
        printError();
    }
    command  = strtok(temp, "&");
    while(command != NULL){
        if (command != '\0'){
            args[n_commands] = trim(command);
            n_commands++;
            //printf("command: %s\n", command); 
            command = strtok(NULL, "&"); 
            if (n_commands >= BUFF_SIZE){
                printError();
                break;
            }
        }
        
    }
    args[n_commands] = NULL;

    // for (int i = 0; i < numArgs; i++){
    //     printf("here: %s", args[i]);
    // }
    // printf("\n");

    return args;
}

//----------------------------
    // clean
    // Description: free memory and close files
//-----------------------------
void clean(void){
    free(line);
    fclose(inpFile);
}


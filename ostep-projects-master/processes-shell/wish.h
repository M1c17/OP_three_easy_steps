/*
 * This file contains the definitions for the wish library;
 * only the ones external programs need to know about lie here.
 *
 * See wish.c for more full detail of the implementations
 */
#include <string.h>
#include <ctype.h>       // isspace

#define INTERACTIVE_MODE 1
#define BATCH_MODE 2
#define BUFF_SIZE 256
#define MAXERR 30
#define DELIMITERS " \n\t>"

//----------------------------
    // printError()
    // Description: 
    // 
    // 
//----------------------------
void printError(void){
    char error_message[MAXERR] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}

//----------------------------
    // print_prompt()
    // Description: 
    // 
    // 
//----------------------------
void print_prompt(void){
    write(STDERR_FILENO, "wish> ", strlen("wish> "));
}
//----------------------------
    // Trim
    // Description: remove leading and trialing space
    // the operators(redirection and parallel commands)
    // do not require whitespace.
//-----------------------------
char *trim(char *s){

    // trim leading spaces
    while(isspace(*s)){
        s++;
    }
    // empty string
    if (*s == '\0'){
        return s;
    }

    //trim trailing spaces
    char *end_line = s + strlen(s) - 1;
    while (end_line > s && isspace(*end_line)){
        end_line--;
    }
    end_line[1] = '\0';
    return s;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024
#define MAXLINE 100

char *read_line(void);

int main(void){
    char **args = malloc(BUFF_SIZE * sizeof(char*));
    char *command;
    char *line;

    line = read_line();
    printf("%s\n", line);
    int position = 0;
    command  = strtok(line, " \n\t>");
    // command = strtok(line, ">");
    printf("%s", command);

    while(command != NULL){
        args[position] = command;
        position++;
        printf("command: %s\n", command); 
        command = strtok(NULL, " \n\t> "); 
    }
    printf("%d\n", position);
    args[position] = NULL;
    
    for (int i = 0; i < position; i++){
        printf("%s\n", args[i]);
    }
    printf("\n"); 

    char *paths[BUFF_SIZE] = {"/bin", NULL};
    char path[BUFF_SIZE];
    int i = 0; 

    while (paths[i] != NULL){
        snprintf(path, BUFF_SIZE, "%s/%s", paths[i], args[1]);
        i++;
    }
    printf("string: %s\n", path);
    printf("\n");    
}

char *read_line(void)
{
  int bufsize = BUFF_SIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += BUFF_SIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


void runInParallel(char **args, char *command, int n_command){
    char *temp = line;

    while ((command = strtok(temp, "&") != NULL){
        if (command != '\0'){
            args[n_command++] = command;
            if (n_command >= BUFF_SIZE){
                printError();
                break;
            }
        }
    }

    for (int i = 0; i < n_command; i++){
        printf("%s\n", args[i]);
    }
    printf("\n"); 
}


// char const *needle = ">";
// size_t needle_length = strlen(needle);
// for (size_t i = 0; i < needle_length; i++){
//     // redirection write file if find ls -la /tmp > output`
//     // find file in commnad line 
//     char const *str = args[i];
//     char const *needle_pos = strstr(str, needle);
//     sscanf(needle_pos + needle_length, "%9s", word);
//     //printf("%s\n", word);
// }

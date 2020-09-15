#include <stddef.h>  // size_t
#include <stdlib.h>  // EXIT_FAILURE
#include <ctype.h>   // isspace()
#include <string.h>  // strlen(), strstr(), sscanf()
#include <stdio.h>   // printf(), fprintf()

int main(void)
{
    char const *str = "ls -la /tmp > output";
    char const *needle = ">";
    size_t needle_length = strlen(needle);
    char const *needle_pos = strstr(str, needle);

    // not found, at end of str or not preceded and followed by whitespace:
    if (!needle_pos || !needle_pos[needle_length] || !isspace((char unsigned)needle_pos[needle_length]) ||
        (needle_pos != str && !isspace((char unsigned)needle_pos[-1])))
    {
        fprintf(stderr, "\"%s\" couldn't be found. :(\n\n", needle);
        return EXIT_FAILURE;
    }   

    // extract the word following the word at needle_pos:
    char word[100];
    sscanf(needle_pos + needle_length, "%99s", word);
    printf("Found \"%s\" after \"%s\"\n\n", word, needle);
}
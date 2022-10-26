#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "tools.h"

#define BUFFER_SIZE 1024

char* get_dir(){
    int bufsize = BUFFER_SIZE;
    char* buf = malloc(bufsize);
    getcwd(buf, bufsize);  // get current working directory
    // Check if getcwd() failed
    while(!buf){
        // Double the buffer size and try again
        bufsize *= 2;
        buf = realloc(buf, bufsize);
        getcwd(buf, bufsize);
    }

    return buf;
}

// Function that clears leading and trailing whitespace
char* clear_whitespace(char* str){
    // If the string is empty, return it
    if(!str){
        return str;
    }

    // Remove leading whitespace
    while(isspace(*str)){
        str++;
    }

    // If the string is empty, return it
    if(!*str){
        return str;
    }

    // Remove trailing whitespace
    char* end = str + strlen(str) - 1;
    while(end > str && isspace(*end)){
        end--;
    }

    // Write a null terminator after the last non-whitespace character
    *(end + 1) = '\0';

    // Return the string
    return str;
}

// Function that clears the quotes in case a value was a literal string
char* clear_quotes(char* str){
    // If the string has quotes, remove them and return the string
    if(str[0] == '"' && str[strlen(str) - 1] == '"'){
        str[strlen(str) - 1] = '\0';
        return str + 1;
    }

    // Otherwise return the string
    return str;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define CMD_SIZE 32
#define MAX_ARGS 16

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

void print_prompt() {
    char* user = "mikeg"; // getlogin() // get the user name
    char* dir = get_dir();     // get current working directory

    printf("%s@cs345sh/%s$ ", user, dir);

    free(dir); // free the memory allocated for dir
}

void read_command(char* buf, char* command, char** args){
    // Read the command from stdin
    fgets(buf, BUFFER_SIZE, stdin);

    // Remove the newline character from the end of the command
    buf[strlen(buf) - 1] = '\0';

    // Parse the command into arguments
    char* token = strtok(buf, " ");

    // The first token is the command
    strcpy(command, token);

    // The rest of the tokens (if any) are arguments
    for(int i = 0; (token = strtok(NULL, " ")); i++){
        args[i] = token;
    }
}

int main(){
    char* buf;
    char* command;
    char** args;

    while(1){
        buf = malloc(BUFFER_SIZE);
        command = malloc(CMD_SIZE);
        args = malloc(MAX_ARGS);

        print_prompt();
        read_command(buf, command, args);

        free(buf);
        free(command);
        // Free each char* in args, then args itself
        for(int i = 0; args[i] && strcmp(args[i], ""); i++){
            free(args[i]);
        }
        free(args);
    }

    return 0;
}
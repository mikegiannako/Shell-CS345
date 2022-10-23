#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// Initialize and return an empty command
Command_t init_command() {
    Command_t cmd = malloc(sizeof(Command_t));
    cmd->buf = calloc(BUFFER_SIZE, sizeof(char));
    cmd->command = calloc(BUFFER_SIZE, sizeof(char));
    cmd->args = calloc(BUFFER_SIZE, sizeof(char*));
    return cmd;
}

// Free a command and its members
void free_command(Command_t cmd) {
    free(cmd->buf);
    free(cmd->command);
    for(int i = 0; i < BUFFER_SIZE; i++){
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd);
}

Command_t read_command(Command_t cmd) {
    // Parse the command into arguments
    char* token = strtok(cmd->buf, " ");

    // The first token is the command
    strcpy(cmd->command, token);
    cmd->args[0] = malloc(strlen(token) + 1);
    strcpy(cmd->args[0], token);

    // The rest of the tokens (if any) are arguments
    for(int i = 1; (token = strtok(NULL, " ")) && strcmp(token, ""); i++){
        // if token is empty break
        cmd->args[i] = malloc(strlen(token) + 1);
        strcpy(cmd->args[i], token);
    }
    
    return cmd;
}

void execute_cd(Command_t cmd) {
    // Check if the user entered "cd .."
    if(!strcmp(cmd->args[1], "..")){
        // Change to the parent directory
        chdir("..");
    }
    // Check if the user entered "cd <dir>"
    else if(cmd->args[1]){
        // Change to the specified directory
        chdir(cmd->args[1]);
    }
}

int execute_command(Command_t cmd){
    // Check if the user entered "exit"
    if(!strcmp(cmd->command, "exit")) return -1;
    // Check if the user entered "cd"
    if(!strcmp(cmd->command, "cd")){
        execute_cd(cmd);
        return 0;
    }

    // Otherwise, fork a child process to execute the command
    pid_t pid = fork();
    if(pid == 0){
        // Child process
        execvp(cmd->command, cmd->args);
    }

    // Parent process
    waitpid(-1, NULL, 0);

    // Free the command and its arguments
    free_command(cmd);

    return 0;
}
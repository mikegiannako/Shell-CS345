#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// Initialize and return an empty command
Command_t init_command() {
    Command_t cmd = malloc(sizeof(struct Command));
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

//--------------------------------- READ COMMAND ---------------------------------

void parse_setenv(Command_t cmd){
    // Set the command to "assignment"
    strcpy(cmd->command, "assignment");

    // Split the buffer into 2 tokens on the equals sign
    // and pass them as arguments to the command
    char* token = strtok(cmd->buf, "=");
    cmd->args[0] = calloc(strlen(token) + 1, sizeof(char));
    strcpy(cmd->args[0], token);
    
    token = strtok(NULL, "=");  
    cmd->args[1] = calloc(strlen(token) + 1, sizeof(char));
    strcpy(cmd->args[1], token);
}

void read_command(Command_t cmd) {
    // Check the command buffer for assignment
    if(strchr(cmd->buf, '=')){
        parse_setenv(cmd);
        return;
    }

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
}

//-------------------------------- EXECUTE COMMAND --------------------------------

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

void execute_echo(Command_t cmd) {
    // If the argument is an environment variable, print its value
    if(cmd->args[1][0] == '$'){
        char* env = getenv(cmd->args[1] + 1);
        if(env) printf("%s\n", env);
    }
    // Otherwise, print the argument
    else printf("%s\n", cmd->args[1]);
}

int execute_command(Command_t cmd){
    // Check if the user entered "exit"
    if(!strcmp(cmd->command, "exit")) return -1;
    // Check if the user entered "cd"
    if(!strcmp(cmd->command, "cd")){
        execute_cd(cmd);
        return 0;
    }
    // Check if the user entered an assignment
    if(!strcmp(cmd->command, "assignment")){
        // Set the environment variable
        setenv(cmd->args[0], cmd->args[1], 1);
        return 0;
    }
    // Check if the user enetered "echo"
    if(!strcmp(cmd->command, "echo")){
        execute_echo(cmd);
        return 0;
    }

    //Otherwise, fork a child process to execute the command
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
#include "command.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
//#include <sys/wait.h>

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

// Function that parses a command that contains assingment
void parse_setenv(Command_t cmd){
    // Set the command to "assignment"
    strcpy(cmd->command, "assignment");

    // Split the buffer into 2 tokens on the equals sign
    // and pass them as arguments to the command
    char* token = clear_whitespace(strtok(cmd->buf, "="));
    cmd->args[0] = calloc(strlen(token) + 1, sizeof(char));
    strcpy(cmd->args[0], token);
    
    token = clear_whitespace(strtok(NULL, "="));  
    cmd->args[1] = calloc(strlen(token) + 1, sizeof(char));
    strcpy(cmd->args[1], token);
}

// Function that parses a command that contains pipe(s)
void read_pipe_command(Command_t cmd){
    // Set the command to "pipe"
    strcpy(cmd->command, "pipe");

    // Splits the string into tokens on the pipe character
    char* token = clear_whitespace(strtok(cmd->buf, "|"));

    for(int i = 0; token; i++){
        cmd->args[i] = calloc(strlen(token) + 1, sizeof(char));

        // Passes the command as an argument that will then be parsed
        // accodingly by the execute_pipe function
        strcpy(cmd->args[i], token);

        // Continues splitting
        token = clear_whitespace(strtok(NULL, "|"));
    }
}

void read_command(Command_t cmd) {
    // If the buffer contains pipe(s)
    if(strchr(cmd->buf, '|')){
        read_pipe_command(cmd);
        return;
    }

    // Check if the buffer contains an assignment
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

// Function that executes one or multiple pipes
void execute_pipe(Command_t cmd){
    // Get the number of pipes
    int pipes = -1;
    for(int i = 0; cmd->args[i]; i++) pipes++;
    
    // Create an array of pipes
    int pipefds[pipes][2];

    // Create the pipes
    for(int i = 0; i < pipes; i++){
        pipe(pipefds[i]);
    }

    Command_t cmd2;

    // Create a child process for each command
    for(int i = 0; cmd->args[i]; i++){
        // Create a new command
        cmd2 = init_command();
        // Copy the command into the command buffer
        strcpy(cmd2->buf, clear_whitespace(cmd->args[i]));
        // Parse the command
        read_command(cmd2);

        // Create a child process
        pid_t pid = fork();
        if(pid == 0){
            // If this is not the first command of the pipes copy the input
            // from the previous pipe
            if(i != 0){
                dup2(pipefds[i - 1][0], STDIN_FILENO);
            }
            // If this is not the last command of the pipes copy the output
            // to the next pipe
            if(i != pipes){
                dup2(pipefds[i][1], STDOUT_FILENO);
            }

            // Close the pipes
            for(int j = 0; j < pipes; j++){
                close(pipefds[j][0]);
                close(pipefds[j][1]);
            }

            // Execute the command
            execvp(cmd2->command, cmd2->args);
            // If execvp() fails, print an error message and exit
            printf("Error: command not found\n");
            _exit(1);
        }

        // Free the command
        free_command(cmd2);
    }


    // Close the pipes
    for(int i = 0; i < pipes + 1; i++){
        close(pipefds[i][0]);
        close(pipefds[i][1]);
    }

    // Wait for all the pids
    for(int i = 0; i < pipes + 1; i++){
        waitpid(-1, NULL, 0);
    }
}

// Function that executes the "cd" command
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

// Function that executes the "echo" command
void execute_echo(Command_t cmd) {
    // If the argument is an environment variable, print its value
    if(cmd->args[1][0] == '$'){
        char* env = getenv(cmd->args[1] + 1);
        if(env) printf("%s\n", clear_quotes(env));
    }
    // Otherwise, print the argument
    else printf("%s\n", cmd->args[1]);
}

int execute_command(Command_t cmd){
    // Check if the command is "pipe"
    if(!strcmp(cmd->command, "pipe")){
        execute_pipe(cmd);
        return 0;
    }
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
//#include <sys/wait.h>
#include "command.h"
#include "stack.h"

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
    char* user = getlogin(); // get the user name
    char* dir = get_dir();     // get current working directory

    printf("%s@cs345sh/%s$ ", user, dir);

    free(dir); // free the memory allocated for dir
}

Command_t read_command(Command_t cmd) {
    // Read the command from stdin
    fgets(cmd->buf, BUFFER_SIZE, stdin);

    // Remove the newline character from the end of the command
    cmd->buf[strlen(cmd->buf) - 1] = '\0';

    // Parse the command into arguments
    char* token = strtok(cmd->buf, " ");

    // The first token is the command
    strcpy(cmd->command, token);

    // The rest of the tokens (if any) are arguments
    for(int i = 0; (token = strtok(NULL, " ")) && strcmp(token, ""); i++){
        // if token is empty break
        cmd->args[i] = malloc(strlen(token) + 1);
        strcpy(cmd->args[i], token);
    }
    
    return cmd;
}

void execute_cd(Command_t cmd) {
    // Check if the user entered "cd .."
    if(!strcmp(cmd->args[0], "..")){
        // Change to the parent directory
        chdir("..");
    }
    // Check if the user entered "cd <dir>"
    else if(cmd->args[0]){
        // Change to the specified directory
        chdir(cmd->args[0]);
    }
}

int execute_command(Command_t cmd){
    // Check if the user entered "exit"
    if(!strcmp(cmd->command, "exit")) return -1;
    // Check if the user entered "cd"
    else if(!strcmp(cmd->command, "cd")) execute_cd(cmd);
    // Otherwise, fork a child process to execute the command
    else{
        // pid_t pid = fork();
        // if(pid == 0){
        //     // Child process
        //     execvp(command, args);
        // }
        // else{
        //     // Parent process
        //     waitpid(-1, NULL, 0);
        // }
        // Print the ocmmand and arguments
        printf("Command: %s\n", cmd->command);
        for(int i = 0; cmd->args[i]; i++){
            // Print ascii value of each character in the argument
            printf("Argument[%d]: %s   ", i, cmd->args[i]);
        }
    }

    // Free the command and its arguments
    free_command(cmd);

    return 0;
}

int main(){
    // Stack that will save the commands to be executed
    Stack_t todo = malloc(sizeof(Stack_t));
    init(todo);

    while(1){
        // If there are no more commands waiting to be executed
        // ask for input from the user
        if(isEmpty(todo)){
            // Print the prompt
            print_prompt();
            // read_line()
            push(todo, read_command(init_command()));
        } 
        // Else execute the next command in the stack
        else{
            // If execute_command returns -1, exit the shell
            if(execute_command(pop(todo)) == -1){
                // Free all the commands in the stack
                while(!isEmpty(todo)){
                    free_command(pop(todo));
                }
                // Free the stack
                free(todo);

                return 0;
            }
        }
    }

    return 0;
}
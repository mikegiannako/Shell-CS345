#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "command.h"
#include "tools.h"
#include "stack.h"

// Recursive function that adds the commands parsed by read_commands
// to the stack in reverse order
void add_to_stack(Stack_t stack, char* token, char* saveptr){
    if(!token) return;
    // Initialize a new command
    Command_t cmd = init_command();

    // Copy the token into the command buffer
    strcpy(cmd->buf, token);

    // Read the command
    read_command(cmd);

    // The reverse order is achieved by first calling the function
    // recursively and then pushing the command to the stack

    // Call for the next token
    char* temp = strtok_r(NULL, ";", &saveptr);
    add_to_stack(stack, temp, saveptr);

    // Push the command to the stack
    push(stack, cmd);
}

// Function that reads stdin and separates it into commands on semi-colons
// Then adds the commands to the stack
void read_commands(Stack_t stack){
    int bufsize = BUFFER_SIZE;
    char* buf = malloc(bufsize);

    char* saveptr;

    // Read stdin
    fgets(buf, bufsize, stdin);

    // Remove the newline character
    buf[strlen(buf) - 1] = '\0';

    // Parse the commands into tokens on semi-colons
    char* token = strtok_r(buf, ";", &saveptr);
    add_to_stack(stack, token, saveptr);

    // Free the buffer
    free(buf);
}

void print_prompt() {
    // get the user name
    char* user = getlogin();   

    // get current working directory 
    char* dir = get_dir();      

    printf("\033[1;33m%s@cs345sh/%s$\033[0m ", user, dir);

    // free the memory allocated for dir
    free(dir);                   
}

int main(){
    // Stack that will save the commands to be executed
    Stack_t todo = stack_init();

    while(1){
        // If there are no more commands waiting to be executed
        // ask for input from the user
        if(isEmpty(todo)){
            // Print the prompt
            print_prompt();
            // read_line()
            read_commands(todo);

            continue;
        } 

        // Else execute the next command in the stack

        // If execute_command returns -1, exit the shell
        if(execute_command(pop(todo)) == -1){
            printf("exit\n");
            // Free all the commands in the stack
            while(!isEmpty(todo)){
                free_command(pop(todo));
            }
            // Free the stack
            free(todo);

            exit(0);
        }
    }

    return 0;
}
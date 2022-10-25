#ifndef _COMMAND_H_
#define _COMMAND_H_

#define BUFFER_SIZE 1024
#define CMD_SIZE 64
#define MAX_ARGS 64

// Struct that saves the info for a command
typedef struct Command {
    char* buf;
    char* command;
    char** args;
}* Command_t; 

// Initialize and return an empty command
Command_t init_command();

// Frees the buffer command and arguments as well as the instance itself
void free_command(Command_t cmd);

// Reads the command and its arguments and executes it
// Returns 0 if successful, -1 if the command was "exit"
int execute_command(Command_t cmd);

// Reads the command from the buffer and parses it into command and arguments
void read_command(Command_t cmd);

#endif // _COMMAND_H_
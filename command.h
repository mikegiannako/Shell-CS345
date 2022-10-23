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

Command_t init_command();

void free_command(Command_t cmd);
int execute_command(Command_t cmd);
Command_t read_command(Command_t cmd);
void execute_cd(Command_t cmd);

#endif // _COMMAND_H_
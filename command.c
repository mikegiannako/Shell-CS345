#include "command.h"
#include <stdlib.h>

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
#ifndef _STACK_H_
#define _STACK_H_

#include "command.h"
#define STACK_SIZE 64

// Stack that saves the commands
typedef struct Stack {
    Command_t commands[STACK_SIZE];
    int top;
    int size;
}* Stack_t;

void init(Stack_t stack);
void push(Stack_t stack, Command_t command);
Command_t pop(Stack_t stack);
int isEmpty(Stack_t stack);

#endif // _STACK_H_ 
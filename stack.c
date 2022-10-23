#include "stack.h"
#include "command.h"
#include <stdio.h>

// Function that initializes the stack
void init(Stack_t stack) {
    stack->top = -1;
    stack->size = 0;
}

// Function that pushes a command to the stack
void push(Stack_t stack, Command_t command) {
    if (stack->size == STACK_SIZE) {
        printf("Stack is full");
        return;
    } else {
        stack->top++;
        stack->size++;
        stack->commands[stack->top] = command;
    }
}

// Function that pops a command from the stack
Command_t pop(Stack_t stack) {
    if (stack->size == 0) {
        printf("Stack is empty");
        return NULL;
    } else {
        Command_t command = stack->commands[stack->top];
        stack->top--;
        stack->size--;
        return command;
    }
}

// Functiont that checks if stack is empty
int isEmpty(Stack_t stack) {
    return stack->size == 0;
}
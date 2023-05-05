#ifndef RSTACK_H_
#define RSTACK_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ArrayStack 
{
  int top;
  int capacity;
  char* array;
} Stack;

Stack* stack;

Stack* createStack(int capacity);
int isEmpty (Stack* stack); 
int isFull (Stack* stack);
char peek (Stack* stack);
void push(Stack *stack, char data);
char pop(Stack* stack);

#endif

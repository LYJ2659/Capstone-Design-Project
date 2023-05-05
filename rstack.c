#include "rstack.h"

Stack* createStack(int capacity) 
{
  stack = (Stack*)malloc(sizeof(Stack));
  if (!stack)
    return NULL;
  stack->capacity = capacity;
  stack->top = -1;
  stack->array = (char*)malloc(stack->capacity * sizeof(char));
  return stack;
}

int isEmpty (Stack* stack) 
{
  return (stack->top == -1);
}

int isFull (Stack* stack) 
{
  return (stack->top == stack->capacity - 1);
}

char peek (Stack* stack) 
{
  if(isEmpty(stack))
    return 0;  
  return stack->array[stack->top];
}

void push(Stack *stack, char data) 
{
  if (isFull(stack))
  {
  	//printf("Stack is full\n");
  }
    
  else 
  {
    stack->array[++stack->top] = data;
    //printf("%d pushed to stack\n", data);
  }
}

char pop(Stack* stack) 
{
  if (isEmpty(stack)) 
  {
    //printf("Stack is empty\n");
    return 0;
  }
  else 
    return (stack->array[stack->top--]);
}


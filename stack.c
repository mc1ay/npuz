/**
 * @file    stack.c
 * @brief   Stack functions for n-puzzle solver program
 * @author  Mitchell Clay
 * @date    4/18/2020
**/

#include "node.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void push(struct Node node, struct Element** stack) {
    struct Element* element = (struct Element*)malloc(sizeof(struct Element));
    element -> node = node;
    element -> next = *stack;
    (*stack) = element;
}

void pop(struct Element** stack) {
    if (*stack != NULL) {
        printf("Element popped\n");
        struct Element* tempPtr = *stack;
        *stack = (*stack) -> next;
        free(tempPtr);
    }
}

void top(struct Element* stack) {
    if (stack != NULL) {
        printf("Element on top of stack (add code for this later)\n");
    }
    else {
        printf("The stack is empty\n");
    }
}
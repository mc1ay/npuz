/**
 * @file    stack.c
 * @brief   Stack functions for n-puzzle solver program
 * @author  Mitchell Clay
 * @date    4/18/2020
**/

#include <stdbool.h>
#include "node.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void push(struct Node *node, struct Element** stack, bool debug) {
    struct Element* element = (struct Element*)malloc(sizeof(struct Element));
    if (debug) {
        printf("Pushing node: %p\n", node);
        printf("Top of stack: %p\n", *stack);
        printf("Next element: %p\n\n", element);
    }
    element -> node = *node;
    element -> next = *stack;
    (*stack) = element;
}

void pop(struct Element** stack, bool debug) {
    if (*stack != NULL) {
        printf("Element popped\n");
        struct Element* tempPtr = *stack;
        *stack = (*stack) -> next;
        free(tempPtr);
    }
}

void top(struct Element* stack, unsigned puzzle_size, bool debug) {
    if (stack != NULL) {
        struct Node node = stack -> node;
        printf("Top element is: \n");
        printNodeInfo(&node, puzzle_size);
    }
    else {
        printf("The stack is empty\n");
    }
}
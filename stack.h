/**
 * @file    stack.h
 * @brief   Stack functions for n-puzzle solver program
 * @author  Mitchell Clay
 * @date    4/18/2020
**/

#ifndef stack_H
#define stack_H

#include "node.h"

struct Element {
    struct Node node;
    struct Element* next;
};

void push(struct Node, struct Element**);
void pop(struct Element**);
void top(struct Element*);

#endif
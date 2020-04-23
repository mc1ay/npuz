/**
 * @file    solve.c
 * @brief   Solution algorithms for n-puzzle problem
 *
 * @author  Mitchell Clay
 * @date    4/22/2020
**/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "solve.h"
#include "state.h"
#include "node.h"
#include "stack.h"

void BruteForce(struct Node* root, unsigned* final, unsigned puzzle_size, bool verbose, bool debug) {
    // make stack and push root node onto it
    struct Element* stack_root = (struct Element*)malloc(sizeof(struct Element));
    stack_root->node = root;
    stack_root->next = NULL;

    int counter = 0;
    
    struct Node* temp_node = NULL;
    
    while (!StackEmpty(stack_root)) {
        temp_node = pop(&stack_root, debug);
        for (int i = 0; i < 4; i++) {
            if (CheckValidMove(puzzle_size, temp_node->blank_position, i) == true) {
                struct Node* new_node = 
                            newNode(new_node, 
                            temp_node->arr, 
                            puzzle_size, 
                            i, 
                            temp_node->blank_position, 
                            temp_node->level + 1, 
                            temp_node,
                            debug);
                new_node->cost = calculateCost(new_node->arr, final, puzzle_size);
                if (debug) {
                    if (i == 0) {
                        printf("Direction: Up ");
                    }
                    else if (i == 1) {
                        printf("Direction: Down ");
                    }
                    else if (i == 2) {
                        printf("Direction: Left ");
                    }
                    else if (i == 3) {
                        printf("Direction: Right ");
                    }
                    printf("Parent: %p Cost: %d Level: %d\n", new_node->parent, new_node->cost, new_node->level);
                }
                if (new_node->cost == 0) {
                    printf("Solution found!\n");
                    printf("Move sequence: ");
                    // put code here to empty stack, then push each node from here to root into stack,
                    // then pop off and print direction
                    return;
                }
                if (new_node->level < 60) {
                    push(&new_node, &stack_root, debug); 
                }
            }
        }               
    }
}


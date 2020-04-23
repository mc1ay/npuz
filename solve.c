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
                /*
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
                */
                if (new_node->cost == 0) {
                    printf("Solution found!\n");
                    printf("Move sequence: ");
                    while (!StackEmpty(stack_root)) {
                        pop(&stack_root, debug);
                    }
                    // push this node onto the clean stack
                    temp_node = new_node;
                    push(&temp_node, &stack_root, debug);
                    // traverse the tree, pushing each parent into stack
                    while (temp_node->parent != NULL) {
                        temp_node = temp_node->parent;
                        push(&temp_node, &stack_root, debug);
                    }
                    //now pop each node and print its direction
                    while (!StackEmpty(stack_root)) {
                        temp_node = pop(&stack_root, debug);
                        if (verbose) {
                            if (temp_node->direction == 0) {
                                printf("U ");
                            }
                            else if (temp_node->direction == 1) {
                                printf("D ");
                            }
                            else if (temp_node->direction == 2) {
                                printf("L ");
                            }
                            else if (temp_node->direction == 3) {
                                printf("R ");
                            }
                        }
                    }
                    if (verbose) {
                        printf("\n");
                    }                        
                    return;
                }
                if (new_node->level < 20) {
                    push(&new_node, &stack_root, debug); 
                }
            }
        }               
    }
}


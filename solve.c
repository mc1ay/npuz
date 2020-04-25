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
#include <limits.h>
#include "solve.h"
#include "state.h"
#include "node.h"
#include "stack.h"

void Solve(struct Node* root, unsigned* final, unsigned puzzle_size, bool verbose, bool debug) {
    int depth = 1;
    int depth_limit = 20;
    struct Node* temp_node = NULL;

    while (depth <= depth_limit) {
        if (debug) {
            printf("Using depth: %d\n", depth);
            printf("Press a key\n");
            char temp = fgetc(stdin);
        }
        // make stack and push root onto it
        struct Element* stack_root = (struct Element*)malloc(sizeof(struct Element));
        // made second stack to keep up with nodes that need to be deleted
        struct Element* needs_freed = (struct Element*)malloc(sizeof(struct Element));

        stack_root->node = root;
        stack_root->next = NULL;
        needs_freed->node = root;
        needs_freed->next = NULL;
                
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
                    //printf("Made node: %p\n", newNode);

                    if (new_node->cost == 0) {
                        PrintSolution(temp_node, new_node, stack_root, verbose, debug);
                        return;
                    }
                    else if (new_node->level < depth) {
                        push(&new_node, &stack_root, debug); 
                    }
                    else {
                        free(new_node);
                    }
                }
            }
            // push temp onto second stack to be freed
            push(&temp_node, &needs_freed, debug);               
        }
        // free whatever we don't need
        temp_node = pop(&needs_freed, debug);
        while (!StackEmpty(needs_freed) && needs_freed->next != NULL) {
                free(temp_node);
                temp_node = pop(&needs_freed, debug);
        }    
        free(stack_root);
        free(needs_freed);
        depth++;    
    }
}

void PrintSolution(struct Node* temp_node, 
                   struct Node* new_node,
                   struct Element* stack_root, 
                   bool verbose, 
                   bool debug) {
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
}

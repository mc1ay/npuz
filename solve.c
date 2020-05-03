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
#include <string.h>
#include <limits.h>
#include "solve.h"
#include "state.h"
#include "node.h"
#include "stack.h"

void Solve(unsigned* initial_state, unsigned* final, unsigned puzzle_size, unsigned blank_position, bool verbose, bool debug) {
    int depth_limit = 100;
    // temporary node
    struct Node* temp_node = NULL;
    // temporary node for holding parent node
    struct Node* parent_node = NULL;
    // array for holding child node pointers for sorting before pushing into stack
    struct Node* child_nodes[4];
    struct Node* root = MakeRootNode(initial_state, final, puzzle_size, blank_position, verbose, debug);
    int depth = root->cost;

    while (depth <= depth_limit) {
        if (verbose) {
            printf("Using depth: %d\n", depth);
        }
        // make stack and push root onto it
        struct Element* stack_root = (struct Element*)malloc(sizeof(struct Element));
        // made second stack to keep up with nodes that need to be checked for deletion
        struct Element* stack_deletion = (struct Element*)malloc(sizeof(struct Element));

        stack_root->node = root;
        stack_root->next = NULL;

        stack_deletion->node = root;
        stack_deletion->next = NULL;
                
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
                    temp_node->children[i] = new_node;
                    
                    child_nodes[i] = new_node;

                    if (new_node->cost == 0) {
                        PrintSolution(temp_node, new_node, stack_root, verbose, debug);
                        return;
                    }
                }
                else {
                    child_nodes[i] = NULL;
                }
            }
            // sort child nodes
            for (int i = 0; i < 3; i++) {
                for (int j = i + 1; j < 4; j++) {
                    if (child_nodes[i] != NULL && child_nodes[j] != NULL) {
                        if (child_nodes[i]->cost > child_nodes[j]->cost) {
                            struct Node* temp = child_nodes[i];
                            child_nodes[i] = child_nodes[j];
                            child_nodes[j] = temp;
                        }
                    }
                    else if (child_nodes[j] == NULL) {
                        struct Node* temp = child_nodes[i];
                        child_nodes[i] = child_nodes[j];
                        child_nodes[j] = temp;
                    }
                  
                }
            }
            
            for (int i = 0; i < 4; i++) {
                if (child_nodes[i] != NULL) {
                    if (child_nodes[i]->level < depth) {
                        push(&child_nodes[i], &stack_root, debug);
                    }
                    else {
                        push(&child_nodes[i], &stack_deletion, debug);
                        while (!StackEmpty(stack_deletion)) {
                            int children_left = 0;
                            temp_node = pop(&stack_deletion, debug);
                            // check to see if last child of parent, if so push
                            // parent onto stack
                            if (temp_node->parent != NULL) {
                                parent_node = temp_node->parent;
                                parent_node->children[temp_node->direction] = NULL;
                                free(temp_node);
                                for (int n = 0; n < 4; n++) {
                                    if (parent_node->children[n] != NULL) {
                                        children_left++;
                                    }
                                }
                                if (children_left == 0) {
                                    push(&parent_node, &stack_deletion, debug);
                                }
                            }
                        }                        
                    }
                }
            }              
        }    
        free(stack_root);
        depth++;  
    }
}

void PrintSolution(struct Node* temp_node, 
                   struct Node* new_node,
                   struct Element* stack_root, 
                   bool verbose, 
                   bool debug) {
    if (verbose) {
        printf("Solution found!\n");
        printf("Move sequence: ");
    }
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

struct Node* MakeRootNode(unsigned* arr, unsigned* arr_final, unsigned puzzle_size, unsigned blank_position, bool verbose, bool debug) {
    // store Initial State as root node
    if (verbose) {
        printf("Storing Initial state as root node\n");    
        printf("\n");
    }
    struct Node* root = (struct Node*) malloc(sizeof(struct Node) + sizeof(unsigned) * puzzle_size * puzzle_size); 
    root->parent = NULL;
    root->blank_position = blank_position;
    root->cost = calculateCost(arr, arr_final, puzzle_size);
    root->level = 0;
    root->direction = -1;
    memcpy(root->arr, arr, sizeof(unsigned) * puzzle_size * puzzle_size); 
    for (int i = 0; i < 4; i++) {
        root->children[i] = NULL;
    }

    if (debug) {
        printf("Root node info:\n");
        printNodeInfo(root, puzzle_size);
        printf("\n");
    }

    return root;
}
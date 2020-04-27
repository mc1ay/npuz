/**
 * @file    solve.cl
 * @brief   OpenCL kernel for n-puzzle solution algorithm
 *
 * @author  Mitchell Clay
 * @date    4/27/2020
**/

#include "solve.h"
#include "state.h"
#include "node.h"
#include "stack.h"

__kernel void solve(__global unsigned* initial, 
                    __global unsigned* final, 
                    __global unsigned* puzzle_size, 
                    __global bool* verbose, 
                    __global bool* debug) {
    int depth = 0;
    int depth_limit = 100;
    // temporary node
    struct Node* temp_node = NULL;
    // temporary node for holding parent node
    struct Node* parent_node = NULL;
    // array for holding child node pointers for sorting before pushing into stack
    struct Node* child_nodes[4];

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
                        // TO-DO : 
                        // notify other PEs that solution has been found and 
                        // somehow pass path back to host here 
                        printf("Solution found\n");
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


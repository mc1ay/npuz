/**
 * @file    node.c
 * @brief   Node specific functions for n-puzzle problem
 *
 * @author  Mitchell Clay
 * @date    4/9/2020
**/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "node.h"
#include "state.h"

// Function to calculate the number of misplaced tiles 
// ie. number of non-blank tiles not in their goal position 
int calculateCost(unsigned* initial, unsigned* final, unsigned puzzle_size) {
    unsigned count = 0;
    unsigned n = puzzle_size * puzzle_size; 
    
    for (int i = 0; i < n; i++) {
        if (initial[i] != final[i]) {
            count++; 
        }
    }
        
    return count; 
} 

struct Node *newNode(struct Node *node, unsigned *arr, unsigned puzzle_size, unsigned direction,
              unsigned old_blank_position, int level, struct Node *parent, bool debug) {
    node = (struct Node*) malloc(sizeof(struct Node) + sizeof(unsigned) * puzzle_size * puzzle_size); 
    //*structP = (struct flexArrayStruct *) malloc(sizeof(struct flexArrayStruct)+ sizeof(int) * array_size);
  
    // set pointer for path to root 
    node->parent = parent; 
  
    // copy puzzle grid from parent node to current node 
    //node->arr = malloc(sizeof *(node->arr) * puzzle_size * puzzle_size);
    memcpy(node->arr, arr, sizeof(unsigned) * puzzle_size * puzzle_size); 
  
    // set number of misplaced tiles 
    node->cost = INT_MAX; 
  
    // set number of moves so far 
    node->level = level; 
  
    // update blank tile location 
    node->blank_position = MoveBlank(node->arr, puzzle_size, old_blank_position, direction, debug);  
  
    return node; 
} 

void printNodeInfo(struct Node* node, unsigned puzzle_size) {
    printf("Node address: %p\n", node);
    printf("Parent node: %p\n", node->parent);
    printf("Memory usage: %lu\n", sizeof(*node) + sizeof(unsigned) * puzzle_size * puzzle_size);
    printf("Cost: %d\n", node->cost);
    printf("Level: %d\n", node->level);
    printf("Blank Position: %d\n", node->blank_position);
    printf("Puzzle state: ");
    for (int i = 0; i < puzzle_size * puzzle_size; i++) {
        printf("%d ", node->arr[i]);
    }
    printf("\n");
}
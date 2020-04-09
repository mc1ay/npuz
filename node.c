/**
 * @file    node.c
 * @brief   Node specific functions for n-puzzle problem
 *
 * @author  Mitchell Clay
 * @date    4/9/2020
**/

#include <stdbool.h>
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
              unsigned blank_position, int level, struct Node *parent) {
    node = malloc(sizeof(*node) + sizeof(unsigned) * puzzle_size * puzzle_size); 
  
    // set pointer for path to root 
    node->parent = parent; 
  
    // copy puzzle grid from parent node to current node 
    memcpy(node->arr, arr, sizeof node->arr); 
  
    // set number of misplaced tiles 
    node->cost = INT_MAX; 
  
    // set number of moves so far 
    node->level = level; 
  
    // update blank tile location 
    node->blank_position = MoveBlank(arr, puzzle_size, blank_position, direction);  
  
    return node; 
} 
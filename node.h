/**
 * @file    node.h
 * @brief   Node specific functions for n-puzzle problem
 *
 * @author  Mitchell Clay
 * @date    4/9/2020
**/

#ifndef node_H
#define node_H

struct Node {
    // address of parent Node struct that this node was
    // generated from
    struct Node *parent; 
  
    // stores blank tile position 
    unsigned blank_position; 
  
    // count of how many elements are not in final position 
    int cost; 
  
    // number of moves from initial state to get here 
    int level; 

    // row-major representation of puzzle grid state 
    unsigned arr[]; 
};

int calculateCost(unsigned*, unsigned*, unsigned);
struct Node *newNode(struct Node*, unsigned*, unsigned, unsigned, unsigned, int, struct Node*, bool); 
void printNodeInfo(struct Node*, unsigned);

#endif
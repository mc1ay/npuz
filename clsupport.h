/**
 * @file    clsupport.h
 * @brief   OpenCL helper functions for n-puzzle solver program
 * @author  Mitchell Clay
 * @date    4/18/2020
**/

#ifndef clsupport_H
#define clsupport_H

struct CLNode {
    // address of parent Node struct that this node was
    // generated from
    unsigned parent; 
  
    // stores blank tile position 
    unsigned blank_position; 
  
    // count of how many elements are not in final position 
    int cost; 
  
    // number of moves from initial state to get here 
    int level; 

    // direction blank moved to get here
    int direction;

    // child nodes
    int children[4];

    // row-major representation of puzzle grid state 
    unsigned arr[9]; 
};

void print_ret_status(int);
int print_cl_devices();
void CLSolve(unsigned*, unsigned*, unsigned, unsigned, bool, bool);

#endif

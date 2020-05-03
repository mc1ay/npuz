/**
 * @file    solve.h
 * @brief   Solution algorithms for n-puzzle problem
 *
 * @author  Mitchell Clay
 * @date    4/22/2020
**/

#ifndef solve_H
#define solve_H

#include "node.h"
#include "stack.h"

void Solve(unsigned*, unsigned*, unsigned, unsigned, bool, bool);
void PrintSolution(struct Node*, struct Node*, struct Element*, bool, bool);
struct Node* MakeRootNode(unsigned*, unsigned*, unsigned, unsigned, bool, bool);

#endif
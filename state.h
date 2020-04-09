/**
 * @file    state.h
 * @brief   State specific functions for n-puzzle problem
 *
 * @author  Mitchell Clay
 * @date    4/9/2020
**/

#ifndef state_H
#define state_H

void PrintState(unsigned*, unsigned);
bool CheckValidMove(unsigned, unsigned, unsigned);
unsigned MoveBlank(unsigned*, unsigned, unsigned, unsigned);
void GenerateInitialState(unsigned*, unsigned, unsigned, bool, bool);

#endif
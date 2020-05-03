/**
 * @file    clsupport.h
 * @brief   OpenCL helper functions for n-puzzle solver program
 * @author  Mitchell Clay
 * @date    4/18/2020
**/

#ifndef clsupport_H
#define clsupport_H

#include "node.h"
#include <stdbool.h>

void print_ret_status(int);
int print_cl_devices();
void CLSolve(unsigned*, unsigned*, unsigned, unsigned, bool, bool);

#endif

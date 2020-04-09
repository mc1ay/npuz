#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <CL/cl.h>
#include <time.h>
#include "state.h"
#include "node.h"

#define MAX_SOURCE_SIZE (0x10000)

void PrintReturnStatus(int ret) {
    if(ret == CL_SUCCESS) {
        printf("OK\n");
    }
    else {
        printf("ERROR: %d\n", ret);
    }
}

int main(void) {
    // initialization 
    // later, update this to accept command line switches
    // puzzle_size = square root of n
    // dimensions = number of dimensions in puzzle
    // n = number of cells (needs to be a integer square)
    // verbose = option to print status messages (bool)
    // seed = random number generator seed
    // arr = row major puzzle grid
    // scramble_size = initial random moves to change inital state
    //                 (to-do: justification for this calculation)
    signed puzzle_size = 3;
    signed dimensions = 2;
    signed n = puzzle_size * puzzle_size;    
    bool verbose = true;   
    bool debug = false;
    unsigned seed = 1;
    unsigned arr[n];
    unsigned scramble_size = 10 * n;

    // seed random number generator
    srand(seed);

    if (verbose) {
        printf("Starting program\n"); 
    }    
    
    if (verbose) {
        printf("Generating initial state\n");
    }
    GenerateInitialState(arr, puzzle_size, scramble_size, verbose, debug);

    if (verbose) {
        printf("Initial state: \n");
        PrintState(arr, puzzle_size);
    }
    
    if (verbose) {
        printf("Done.\n");
    }

    return 0;
}
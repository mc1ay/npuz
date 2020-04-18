/**
 * @file    main.c
 * @brief   Main for n-puzzle solver program
 * @author  Mitchell Clay
 * @date    4/9/2020
**/

#include <CL/cl.h>
#include <ctype.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include "state.h"
#include "node.h"
#include "clsupport.h"

#define MAX_SOURCE_SIZE (0x10000)

int main(int argc, char **argv) {
    // initialization 
    // puzzle_size = square root of n
    // dimensions = number of dimensions in puzzle
    // verbose = option to print status messages (bool)
    // seed = random number generator seed
    // scramble_size = initial random moves to change inital state
    //                 (to-do: justification for this calculation)
    signed puzzle_size = 3;
    signed dimensions = 2;
    bool verbose = false;   
    bool debug = false;
    unsigned seed = 1;
    unsigned scramble_size = 0;

    // get command line switches
    int c;
    while ((c = getopt(argc, argv, "dvn:s:c:m:")) != -1)
    switch (c) {
        case 'd':
            debug = true;
            break;
        case 'v':
            verbose = true;
            break;
        case 'n':
            puzzle_size = atoi(optarg);
            break;
        case 's':
            scramble_size = atoi(optarg);
            break;
        case 'c':
            seed = atoi(optarg);
            break;
        case 'm':
            dimensions = atoi(optarg);
            break;
        case '?':
            if (optopt == 'c')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
                return 1;
        default:
            abort ();
    }

     // calculate n from puzzle size to make parameter passing easier
    signed n = puzzle_size * puzzle_size;   

    // allocate row-major array for initial state
    unsigned arr[n];

    // set scramble_size default if switch wasn't used
    if (scramble_size == 0) {
        scramble_size = 10 * n;
    }

    // if verbose is set, print out program options/defaults
    if (verbose) {
        printf("Program Options/Defaults: \n");
        printf("Debug: ");
        if (debug) {
            printf("true\n");
        }
        else {
            printf("false\n");
        }
        printf("Puzzle size: %d\n", puzzle_size);
        printf("Dimensions: %d\n", dimensions);
        printf("Scramble size: %d\n", scramble_size);
        printf("Seed: %d\n", seed);
    }

    // if verbose is set, print out OpenCL device info
    if (verbose) {
        printf("OpenCL device(s):\n");
        print_cl_devices();
    }

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

#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <CL/cl.h>
#include <time.h>

#define MAX_SOURCE_SIZE (0x10000)

void print_ret_status(int ret) {
    if(ret == CL_SUCCESS) {
        printf("OK\n");
    }
    else {
        printf("ERROR: %d\n", ret);
    }
}

void generate_initial_state(int *arr, int n, bool debug) {
    // array to store whether values have already been generated
    // and initialize
    bool used[n];
    for (int i = 0; i < n; i++) {
        used[i] = false;
    }
    
    // generate an unsigned int between 0 and n - 1 until 
    // finding an unused one and assign it to the next cell
    int randomNumber;
    for (int i = 0; i < n; i++) {
        do {
            randomNumber = rand() % n;
        } while (used[randomNumber]);
        used[randomNumber] = true;
        arr[i] = randomNumber;
        if (debug) {
            printf("%d: %d\n", i, randomNumber);
        }
    }
}

void print_state(int *arr, int puzzleSize) {
    // iterate over two loops, each sqrt(n) to print each
    // column of each row of the grid
    for (int i = 0; i < puzzleSize; i++) {
        for (int j = 0; j < puzzleSize; j++) {
            if (arr[i * puzzleSize + j] == 0) {
                printf(" \t", arr[i * puzzleSize + j]);
            }
            else {
                printf("%d\t", arr[i * puzzleSize + j]);
            }
        }
        printf("\n\n\n");
    }
}

int main(void) {
    // initialization 
    // later, update this to accept command line switches
    // puzzleSize = square root of n
    // n = number of cells (needs to be a integer square)
    // verbose = option to print status messages (bool)
    // seed = random number generator seed
    // arr = row major puzzle grid
    signed puzzleSize = 5;
    signed n = puzzleSize * puzzleSize;    
    bool verbose = true;   
    bool debug = false;
    unsigned seed = 1;
    unsigned arr[n];

    // seed random number generator
    srand(seed);

    if (verbose) {
        printf("Starting program\n"); 
    }    
    
    if (verbose) {
        printf("Generating initial state\n");
    }
    generate_initial_state(arr, n, debug);

    if (verbose) {
        printf("Initial state: \n");
        print_state(arr, puzzleSize);
    }
    
    if (verbose) {
        printf("Done.\n");
    }

    return 0;
}
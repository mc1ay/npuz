#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <CL/cl.h>
#include <time.h>

#define MAX_SOURCE_SIZE (0x10000)

void PrintReturnStatus(int ret) {
    if(ret == CL_SUCCESS) {
        printf("OK\n");
    }
    else {
        printf("ERROR: %d\n", ret);
    }
}

void PrintState(unsigned *arr, unsigned puzzle_size) {
    // iterate over two loops, each sqrt(n) to print each
    // column of each row of the grid
    for (int i = 0; i < puzzle_size; i++) {
        for (int j = 0; j < puzzle_size; j++) {
            if (arr[i * puzzle_size + j] == 0) {
                printf(" \t", arr[i * puzzle_size + j]);
            }
            else {
                printf("%d\t", arr[i * puzzle_size + j]);
            }
        }
        printf("\n\n\n");
    }
}

bool CheckValidMove(unsigned puzzle_size, unsigned blank_position, unsigned direction) {
    bool valid;
    // Top-left corner
    if (blank_position == 0) {
        if (direction == 1 || direction == 3) {
            valid = true;
        }
        else {
            valid = false;
        }
    }
    // Top-right corner
    else if (blank_position == (puzzle_size - 1)) {
        if (direction == 1 || direction == 2) {
            valid = true;
        }
        else {
            valid = false;
        }
    }
    // Bottom-left corner
    else if (blank_position == (puzzle_size * puzzle_size - puzzle_size)) {
        if (direction == 0 || direction == 3) {
            valid = true;
        }
        else {
            valid = false;
        }
    }
    // Bottom-right corner
    else if (blank_position == (puzzle_size * puzzle_size - 1)) {
        if (direction == 0 || direction == 2) {
            valid = true;
        }
        else {
            valid = false;
        }
    }
    // Top row
    else if (blank_position < puzzle_size) {
        // all valid except up
        if (direction == 0) {
            valid = false;
        }
        else {
            valid = true;
        }
    }
    // Bottom row
    else if (blank_position > (puzzle_size * puzzle_size - puzzle_size)) {
        // all valid except down
        if (direction == 1) {
            valid = false;
        }
        else {
            valid = true;
        }
    }
    // Left row
    else if ((blank_position % puzzle_size) == 0) {
        // all valid except left
        if (direction == 2) {
            valid = false;
        }
        else {
            valid = true;
        }
    }
    // Right row
    else if ((blank_position % puzzle_size) == (puzzle_size - 1)) {
        // all valid except right
        if (direction == 3) {
            valid = false;
        }
        else {
            valid = true;
        }
    }
    // all others should be valid
    else {
        valid = true;
    }

    return valid;
}

unsigned MoveBlank(unsigned *arr, unsigned puzzle_size, unsigned blank_position, unsigned direction) {
    // move blank specified direction, and move correct tile into its place
    // 0 - Up
    // 1 - Down
    // 2 - Left
    // 3 - Right
    if (direction == 0) {
        arr[blank_position] = arr[blank_position - puzzle_size];
        arr[blank_position - puzzle_size] = 0;
        blank_position = blank_position - puzzle_size;
    }
    else if (direction == 1) {
        arr[blank_position] = arr[blank_position + puzzle_size];
        arr[blank_position + puzzle_size] = 0;
        blank_position = blank_position + puzzle_size;
    }
    else if (direction == 2) {
        arr[blank_position] = arr[blank_position - 1];
        arr[blank_position - 1] = 0;
        blank_position = blank_position -1;
    }
    else if (direction == 3) {
        arr[blank_position] = arr[blank_position + 1];
        arr[blank_position + 1] = 0;
        blank_position = blank_position + 1;
    }

    return blank_position;
}

void GenerateInitialState(unsigned *arr, unsigned puzzle_size, unsigned scramble_size, bool verbose, bool debug) {
    // store grid size of size^2 in variable n to make code cleaner
    unsigned n = puzzle_size * puzzle_size;

    // set grid state to final state before scrambling
    for (int i = 0; i < n - 1; i++) {
        arr[i] = i + 1;
    }
   
    // set blank tile
    arr[n - 1] = 0;

    // set blank tile position
    unsigned blank_position = n - 1;

    // check layout before scramble
    if (verbose) {
        printf("Grid before scrambling: \n");
        PrintState(arr, puzzle_size);
    }

    // move blank tile scramble_size times randomly but generating random direction 
    // and checking for a valid move
    // 0 - Up
    // 1 - Down
    // 2 - Left
    // 3 - Right
    unsigned direction;
    bool valid_move;
    
    for (int i = 0; i < scramble_size; i++) {
        do {
            direction = rand() % 4;
            valid_move = CheckValidMove(puzzle_size, blank_position, direction);
        } while (!valid_move);
        blank_position = MoveBlank(arr, puzzle_size, blank_position, direction);
        if (debug) {
            printf("moving blank direction: %d\n", direction);
            PrintState(arr, puzzle_size);
        }
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
    signed puzzle_size = 4;
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
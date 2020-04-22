/**
 * @file    state.c
 * @brief   State specific functions for n-puzzle problem
 *
 * @author  Mitchell Clay
 * @date    4/9/2020
**/

#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include "state.h"

void PrintState(unsigned *arr, unsigned puzzle_size) {
    // iterate over two loops, puzzle_size to print each
    // column of each row of the grid
    for (int i = 0; i < puzzle_size; i++) {
        for (int j = 0; j < puzzle_size; j++) {
            // 0 = blank tile, don't print it
            if (arr[i * puzzle_size + j] == 0) {
                printf(" \t");
            }
            else {
                printf("%d\t", arr[i * puzzle_size + j]);
            }
        }
        printf("\n\n\n");
    }
}

unsigned MoveBlank(unsigned *arr, 
                   unsigned puzzle_size, 
                   unsigned blank_position, 
                   unsigned direction,
                   bool debug) {
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

unsigned GenerateInitialState(unsigned *arr, 
                          unsigned *arr_final, 
                          unsigned puzzle_size, 
                          unsigned scramble_size, 
                          bool verbose, 
                          bool debug) {
    // store grid size of size^2 in variable n to make code cleaner
    unsigned n = puzzle_size * puzzle_size;

    // set grid state to final state before scrambling and set up final state
    for (int i = 0; i < n - 1; i++) {
        arr[i] = i + 1;
        arr_final[i] = i + 1;
    }
   
    // set blank tile
    arr[n - 1] = 0;
    arr_final[n - 1] = 0;

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
        blank_position = MoveBlank(arr, puzzle_size, blank_position, direction, debug);
        if (debug) {
            printf("moving blank direction: %d\n", direction);
            PrintState(arr, puzzle_size);
        }
    }

    return blank_position;
}
/**
 * @file    solve.cl
 * @brief   OpenCL kernel for n-puzzle solution algorithm
 *
 * @author  Mitchell Clay
 * @date    4/27/2020
**/

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

__kernel void solve(__global unsigned*, __global unsigned*, __global unsigned*, __global unsigned*, __global struct CLNode*); 
void MakeRootNode(__global struct CLNode*, __global unsigned*, __global unsigned*, __global unsigned*, __global unsigned*);
void PrintNodeInfo(__global struct CLNode*, int, __global unsigned*);
bool CheckValidMove(__global unsigned*, unsigned, unsigned);


__kernel void solve(__global unsigned* initial, 
                    __global unsigned* final, 
                    __global unsigned* puzzle_size, 
                    __global unsigned* blank_position,
                    __global struct CLNode* CLNode_array) {
    unsigned explore_stack[100];
    unsigned delete_stack[100];
    int explore_stack_index = 0;
    int delete_stack_index = 0;
    printf("---------------------------\n");
    printf("| Inside kernel           |\n");
    printf("---------------------------\n");
    printf("Initial array: ");
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        printf("%d ", initial[i]);
    }
    printf("\n");
    int depth = 0;
    int depth_limit = 1;
    MakeRootNode(CLNode_array, initial, final, puzzle_size, blank_position);
    explore_stack[explore_stack_index] = 0;

    while (depth <= depth_limit) {
        printf("Using depth: %d\n", depth);
        explore_stack_index = 0;
        delete_stack_index = 0;
        // Do until stack is empty
        while (explore_stack_index != -1) {
            // Check for valid moves
            for (int i = 0; i < 4; i++) {
                printf("Direction %d returns %d\n", i, CheckValidMove(puzzle_size, CLNode_array[0].blank_position, i));
            }
            explore_stack_index--;
        }
        depth++;
    }
}

void MakeRootNode(
                            __global struct CLNode* CLNode_array,
                            __global unsigned* arr,
                            __global unsigned* arr_final,
                            __global unsigned* puzzle_size,
                            __global unsigned* blank_position) {
    // store Initial State as root node
    printf("Storing Initial state as root node\n");    
    printf("\n");
    int count = 0;
    CLNode_array[0].parent = NULL;
    CLNode_array[0].blank_position = *blank_position;
    CLNode_array[0].level = 0;
    CLNode_array[0].direction = -1;
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        CLNode_array[0].arr[i] = arr[i];
        if (arr[i] != arr_final[i]) {
            count++; 
        }
    }
    CLNode_array[0].cost = count;
    for (int i = 0; i < 4; i++) {
        CLNode_array[0].children[i] = NULL;
    }

    printf("Root node info:\n");
    PrintNodeInfo(CLNode_array, 0, puzzle_size);
    printf("\n");
}

void PrintNodeInfo(__global struct CLNode* CLNode_array, int index, __global unsigned* puzzle_size) {
    printf("Node address: %p\n", CLNode_array[0]);
    printf("Parent node: %p\n", CLNode_array[0].parent);
    printf("Memory usage: %lu\n", sizeof(CLNode_array[0]));
    printf("Cost: %d\n", CLNode_array[0].cost);
    printf("Level: %d\n", CLNode_array[0].level);
    printf("Blank Position: %d\n", CLNode_array[0].blank_position);
    printf("Puzzle state: ");
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        printf("%d ", CLNode_array[0].arr[i]);
    }
    printf("\n");
    printf("Child nodes: ");
    for (int i = 0; i < 4; i++) {
        printf("%p ", CLNode_array[0].children[i]);
    }
    printf("\n");
}

bool CheckValidMove(__global unsigned* puzzle_size, unsigned blank_position, unsigned direction) {
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
    else if (blank_position == (*puzzle_size - 1)) {
        if (direction == 1 || direction == 2) {
            valid = true;
        }
        else {
            valid = false;
        }
    }
    // Bottom-left corner
    else if (blank_position == (*puzzle_size * *puzzle_size - *puzzle_size)) {
        if (direction == 0 || direction == 3) {
            valid = true;
        }
        else {
            valid = false;
        }
    }
    // Bottom-right corner
    else if (blank_position == (*puzzle_size * *puzzle_size - 1)) {
        if (direction == 0 || direction == 2) {
            valid = true;
        }
        else {
            valid = false;
        }
    }
    // Top row
    else if (blank_position < *puzzle_size) {
        // all valid except up
        if (direction == 0) {
            valid = false;
        }
        else {
            valid = true;
        }
    }
    // Bottom row
    else if (blank_position > (*puzzle_size * *puzzle_size - *puzzle_size)) {
        // all valid except down
        if (direction == 1) {
            valid = false;
        }
        else {
            valid = true;
        }
    }
    // Left row
    else if ((blank_position % *puzzle_size) == 0) {
        // all valid except left
        if (direction == 2) {
            valid = false;
        }
        else {
            valid = true;
        }
    }
    // Right row
    else if ((blank_position % *puzzle_size) == (*puzzle_size - 1)) {
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


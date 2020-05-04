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

__kernel void solve(__global unsigned*, __global unsigned*, __global unsigned*, __global unsigned*, __global struct CLNode*, __global unsigned*, __global unsigned*); 
__kernel void MakeRootNode(__global struct CLNode*, __global unsigned*, __global unsigned*, __global unsigned*, __global unsigned*);
__kernel void PrintNodeInfo(__global struct CLNode*, __global unsigned*);


__kernel void solve(__global unsigned* initial, 
                    __global unsigned* final, 
                    __global unsigned* puzzle_size, 
                    __global unsigned* blank_position,
                    __global struct CLNode* CLNode_array,
                    __global unsigned* explore_stack,
                    __global unsigned* delete_stack) {
    unsigned explore_stack_index = 0;
    unsigned delete_stack_index = 0;
    printf("---------------------------\n");
    printf("| Inside kernel           |\n");
    printf("---------------------------\n");
    printf("Initial array: ");
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        printf("%d ", initial[i]);
    }
    printf("\n");
    int depth = 0;
    int depth_limit = 100;
    MakeRootNode(CLNode_array, initial, final, puzzle_size, blank_position);
    explore_stack[explore_stack_index] = 0;
}

__kernel void MakeRootNode(
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
    PrintNodeInfo(CLNode_array, puzzle_size);
    printf("\n");
}

__kernel void PrintNodeInfo(__global struct CLNode* CLNode_array, __global unsigned* puzzle_size) {
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



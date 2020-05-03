/**
 * @file    solve.cl
 * @brief   OpenCL kernel for n-puzzle solution algorithm
 *
 * @author  Mitchell Clay
 * @date    4/27/2020
**/

struct Node {
    // address of parent Node struct that this node was
    // generated from
    struct Node* parent; 
  
    // stores blank tile position 
    unsigned blank_position; 
  
    // count of how many elements are not in final position 
    int cost; 
  
    // number of moves from initial state to get here 
    int level; 

    // direction blank moved to get here
    int direction;

    // child nodes
    struct Node* children[4];

    // row-major representation of puzzle grid state 
    unsigned arr[9]; 
};

__kernel void solve(__global unsigned*, __global unsigned*, __global unsigned*, __global unsigned*); 
__kernel void MakeRootNode(__global struct Node*, __global unsigned*, __global unsigned*, __global unsigned*, __global unsigned*);
__kernel void PrintNodeInfo(__global struct Node*, __global unsigned*);


__kernel void solve(__global unsigned* initial, 
                    __global unsigned* final, 
                    __global unsigned* puzzle_size, 
                    __global unsigned* blank_position) {
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
    __global struct Node* root;
    // Uncommenting either of the following lines causes kernel execution to terminate
    // Unsure how to debug this issue...
    //MakeRootNode(root, initial, final, puzzle_size, blank_position);
    //root->level = 0;
    PrintNodeInfo(root, puzzle_size);
}

__kernel void MakeRootNode(
                            __global struct Node* root,
                            __global unsigned* arr,
                            __global unsigned* arr_final,
                            __global unsigned* puzzle_size,
                            __global unsigned* blank_position) {
    // store Initial State as root node
    printf("Storing Initial state as root node\n");    
    printf("\n");
    int count = 0;
    printf("Setting parent\n");
    root->parent = NULL;
    printf("Setting blank position\n");
    root->blank_position = *blank_position;
    printf("Setting level\n");
    root->level = 0;
    printf("Setting direction\n");
    root->direction = -1;
    printf("Setting state\n");
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        root->arr[i] = arr[i];
        if (arr[i] != arr_final[i]) {
            count++; 
        }
    }
    printf("Setting cost\n");
    root->cost = count;
    for (int i = 0; i < 4; i++) {
        root->children[i] = NULL;
    }

    printf("Root node info:\n");
    PrintNodeInfo(root, puzzle_size);
    printf("\n");
}

__kernel void PrintNodeInfo(__global struct Node* node, __global unsigned* puzzle_size) {
    printf("Node address: %p\n", node);
    printf("Parent node: %p\n", node->parent);
    printf("Memory usage: %lu\n", sizeof(*node));
    printf("Cost: %d\n", node->cost);
    printf("Level: %d\n", node->level);
    printf("Blank Position: %d\n", node->blank_position);
    printf("Puzzle state: ");
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        printf("%d ", node->arr[i]);
    }
    printf("\n");
    printf("Child nodes: ");
    for (int i = 0; i < 4; i++) {
        printf("%p ", node->children[i]);
    }
    printf("\n");
}



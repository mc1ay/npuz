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
    int parent; 
  
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
void NewNode(__global struct CLNode*, __global unsigned*, __global unsigned*, unsigned, int, int);
unsigned MoveBlank(__global struct CLNode*, __global unsigned*, unsigned, unsigned, int);

__kernel void solve(__global unsigned* initial, 
                    __global unsigned* final, 
                    __global unsigned* puzzle_size, 
                    __global unsigned* blank_position,
                    __global struct CLNode* CLNode_array) {
    unsigned explore_stack[100];
    unsigned delete_stack[100];
    int node_index = -1;
    int explore_stack_index = -1;
    int delete_stack_index = -1;
    printf("---------------------------\n");
    printf("| Inside kernel           |\n");
    printf("---------------------------\n");
    printf("Initial array: ");
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        printf("%d ", initial[i]);
    }
    printf("\n");
    int depth = 1;
    int depth_limit = 10;
    MakeRootNode(CLNode_array, initial, final, puzzle_size, blank_position);
    node_index++;
    explore_stack_index++;
    explore_stack[explore_stack_index] = 0;

    while (depth <= depth_limit) {
        printf("Using depth: %d\n", depth);
        explore_stack_index = 0;
        delete_stack_index = -1;
        int generated_nodes = 0;
        int deleted_nodes = 0;
        // Do until stack is empty
        while (explore_stack_index != -1) {
            unsigned current_node = explore_stack[explore_stack_index];
            explore_stack_index--;
            for (int i = 0; i < 4; i++) {
                if (CheckValidMove(puzzle_size, CLNode_array[0].blank_position, i) == true) {
                    node_index++;
                    generated_nodes++;
                    NewNode(CLNode_array, puzzle_size, final, i, current_node, node_index);

                    CLNode_array[current_node].children[i] = node_index;
                    if (CLNode_array[node_index].cost == 0) {
                        printf("Found solution!!!\n");
                        return;
                    }
                }
                else { 
                    CLNode_array[current_node].children[i] = -1;
                }
            }
            // sort child nodes so that lowest cost child will get pushed onto the stack first
            for (int i = 0; i < 3; i++) {
                for (int j = i + 1; j < 4; j++) {
                    if (CLNode_array[current_node].children[i] != -1 && 
                        CLNode_array[current_node].children[j] != -1 ) {
                            if (CLNode_array[CLNode_array[current_node].children[i]].cost > 
                                CLNode_array[CLNode_array[current_node].children[j]].cost) {
                                    int temp = CLNode_array[current_node].children[i];
                                    CLNode_array[current_node].children[i] = 
                                        CLNode_array[current_node].children[j];
                                    CLNode_array[current_node].children[j] = temp;
                            }
                    }
                    else if (CLNode_array[current_node].children[j] != -1) {
                        int temp = CLNode_array[current_node].children[i];
                        CLNode_array[current_node].children[i] = 
                            CLNode_array[current_node].children[j];
                        CLNode_array[current_node].children[j] = temp;
                    }
                }
            }

            // For each child node, push it onto the stack if its level doesn't
            // exceed the current search depth
            for (int i = 0; i < 4; i++) {
                if (CLNode_array[current_node].children[i] != -1) {
                    if (CLNode_array[CLNode_array[current_node].children[i]].level < depth) {
                        explore_stack_index++;
                        explore_stack[explore_stack_index] = CLNode_array[current_node].children[i];
                    }
                    else {
                        delete_stack_index++;

                        delete_stack[delete_stack_index] = CLNode_array[current_node].children[i];
                        while (delete_stack_index != -1) {
                            int children_left = 0;
                            unsigned delete_node = delete_stack[delete_stack_index];
                            delete_stack_index--;
                            if (CLNode_array[delete_node].parent != -1) {
                                // remove node from parent's child array
                                CLNode_array[CLNode_array[delete_node].parent].children[CLNode_array[delete_node].direction] = -1;
                                // remove node from CLNodes array by shifting nodes
                                for (int n = delete_node; n < node_index; n++) {
                                    CLNode_array[n] = CLNode_array[n + 1];  
                                }
                                node_index--;
                                deleted_nodes++;
                                // search parent for more children
                                if (CLNode_array[delete_node].parent != 0) {
                                    for (int n = 0; n < 4; n++) {
                                        if (CLNode_array[CLNode_array[delete_node].parent].children[n] != -1) {
                                            children_left++;
                                        }
                                    }
                                    if (children_left == 0) {
                                        delete_stack_index++;
                                        delete_stack[delete_stack_index] = CLNode_array[delete_node].parent;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        printf("Generated %d nodes and deleted %d nodes\n", generated_nodes, deleted_nodes);
        depth++;
    }
    printf("Depth limit reached\n");
}

void MakeRootNode(__global struct CLNode* CLNode_array,
                  __global unsigned* arr,
                  __global unsigned* arr_final,
                  __global unsigned* puzzle_size,
                  __global unsigned* blank_position) {
    // store Initial State as root node
    printf("Storing Initial state as root node\n");    
    printf("\n");
    int count = 0;
    CLNode_array[0].parent = -1;
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
        CLNode_array[0].children[i] = -1;
    }
}

void PrintNodeInfo(__global struct CLNode* CLNode_array, int index, __global unsigned* puzzle_size) {
    printf("Node index: %d\n", index);
    printf("Parent node: %d\n", CLNode_array[index].parent);
    printf("Cost: %d\n", CLNode_array[index].cost);
    printf("Level: %d\n", CLNode_array[index].level);
    printf("Blank Position: %d\n", CLNode_array[index].blank_position);
    printf("Puzzle state: ");
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        printf("%d ", CLNode_array[index].arr[i]);
    }
    printf("\n");
    printf("Child nodes: ");
    for (int i = 0; i < 4; i++) {
        printf("%d ", CLNode_array[index].children[i]);
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

void NewNode(__global struct CLNode* CLNode_array, 
             __global unsigned* puzzle_size, 
             __global unsigned* arr_final, 
             unsigned direction, 
             int current_node, 
             int index) {
    int count = 0;
    CLNode_array[index].parent = current_node;
    CLNode_array[index].level = CLNode_array[current_node].level + 1;
    CLNode_array[index].direction = direction;
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        CLNode_array[index].arr[i] = CLNode_array[current_node].arr[i];
    }
    CLNode_array[index].cost = count;
    for (int i = 0; i < 4; i++) {
        CLNode_array[index].children[i] = -1;
    }
    CLNode_array[index].blank_position = 
        MoveBlank(CLNode_array, puzzle_size, CLNode_array[current_node].blank_position, direction, index);
    for (int i = 0; i < *puzzle_size * *puzzle_size; i++) {
        if (CLNode_array[index].arr[i] != arr_final[i]) {
            count++; 
        }
    }
    CLNode_array[index].cost = count;
}

unsigned MoveBlank(__global struct CLNode* CLNode_array, 
                   __global unsigned* puzzle_size, 
                   unsigned blank_position, 
                   unsigned direction,
                   int index) {
    // move blank specified direction, and move correct tile into its place
    // 0 - Up
    // 1 - Down
    // 2 - Left
    // 3 - Right
    if (direction == 0) {
        CLNode_array[index].arr[blank_position] = CLNode_array[index].arr[blank_position - *puzzle_size];
        CLNode_array[index].arr[blank_position - *puzzle_size] = 0;
        blank_position = blank_position - *puzzle_size;
    }
    else if (direction == 1) {
        CLNode_array[index].arr[blank_position] = CLNode_array[index].arr[blank_position + *puzzle_size];
        CLNode_array[index].arr[blank_position + *puzzle_size] = 0;
        blank_position = blank_position + *puzzle_size;
    }
    else if (direction == 2) {
        CLNode_array[index].arr[blank_position] = CLNode_array[index].arr[blank_position - 1];
        CLNode_array[index].arr[blank_position - 1] = 0;
        blank_position = blank_position -1;
    }
    else if (direction == 3) {
        CLNode_array[index].arr[blank_position] = CLNode_array[index].arr[blank_position + 1];
        CLNode_array[index].arr[blank_position + 1] = 0;
        blank_position = blank_position + 1;
    }

    return blank_position;
}


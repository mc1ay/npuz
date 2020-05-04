/**
 * @file    clsupport.c
 * @brief   OpenCL helper functions for n-puzzle solver program
 * @author  Mitchell Clay
 * @date    4/18/2020
**/

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "clsupport.h"

#define MAX_SOURCE_SIZE (0x10000)

// Function to print ret status from OpenCL functions
void print_ret_status(int ret) {
    if(ret == CL_SUCCESS) {
        printf("OK\n");
    }
    else {
        printf("ERROR: %d\n", ret);
    }
}

// Print out CL device info
// code is from https://gist.github.com/courtneyfaulkner/7919509
// This will probably be replaced later with more program-specific info
int print_cl_devices() {
    int i, j;
    char* value;
    size_t valueSize;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_uint deviceCount;
    cl_device_id* devices;
    cl_uint maxComputeUnits;

    // get all platforms
    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);

    for (i = 0; i < platformCount; i++) {

        // get all devices
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
        devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

        // for each device print critical attributes
        for (j = 0; j < deviceCount; j++) {

            // print device name
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
            printf("%d. Device: %s\n", j+1, value);
            free(value);

            // print hardware device version
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
            printf(" %d.%d Hardware version: %s\n", j+1, 1, value);
            free(value);

            // print software driver version
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
            printf(" %d.%d Software version: %s\n", j+1, 2, value);
            free(value);

            // print c version supported by compiler for device
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
            printf(" %d.%d OpenCL C version: %s\n", j+1, 3, value);
            free(value);

            // print parallel compute units
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                    sizeof(maxComputeUnits), &maxComputeUnits, NULL);
            printf(" %d.%d Parallel compute units: %d\n", j+1, 4, maxComputeUnits);
        }
        free(devices);
    }
    free(platforms);
    return 0;
}

void CLSolve(unsigned* initial_state, 
             unsigned* final_state, 
             unsigned puzzle_size, 
             unsigned blank_position, 
             bool verbose, 
             bool debug) {
    
    cl_uint maxComputeUnits;
    size_t maxWorkGroupSize;
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    size_t global_item_size = 1;
    size_t local_item_size = 1;
    unsigned num_compute_units = 1;
    unsigned num_pes_per_unit = 1;
    // This needs to allow for at least (2 * dimensions * depth) number of nodes
    unsigned node_array_size = 100;
    
    // Get platform and device information
    printf("Getting platform and device information: ");
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, 
            &device_id, &ret_num_devices);
    print_ret_status(ret);
    
    FILE *fp;
    char *source_str;
    size_t source_size;
    fp = fopen("solve.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );

    // Get Max Compute Units
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS,
                    sizeof(maxComputeUnits), &maxComputeUnits, NULL);
    printf("Parallel compute units: %d\n", maxComputeUnits);

    // Get Max Workgroup Size
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE,
                    sizeof(maxWorkGroupSize), &maxWorkGroupSize, NULL);
    printf("Max workgroup size: %lu\n", maxWorkGroupSize);

    // Create an OpenCL context
    printf("Creating context: "); 
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
    print_ret_status(ret);

    // Create a command queue
    printf("Creating queue: ");
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    print_ret_status(ret);

    // Create memory buffers
    cl_mem initial_state_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            sizeof(unsigned) * puzzle_size * puzzle_size , NULL, &ret);
    printf("Creating initial state buffer: ");
    print_ret_status(ret);
    cl_mem final_state_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            sizeof(unsigned) * puzzle_size * puzzle_size , NULL, &ret);
    printf("Creating final state buffer: ");
    print_ret_status(ret);
    cl_mem puzzle_size_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            sizeof(unsigned), NULL, &ret);
    printf("Creating puzzle size buffer: ");
    print_ret_status(ret);
    cl_mem blank_position_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, 
            sizeof(unsigned), NULL, &ret);
    printf("Creating blank position buffer: ");
    print_ret_status(ret);
    cl_mem CLNodes_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            sizeof(struct CLNode) * num_compute_units * num_pes_per_unit * node_array_size,
            NULL, &ret);
    printf("Creating CLNodes buffer: ");
    print_ret_status(ret);
    cl_mem explore_stack_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            sizeof(unsigned) * num_compute_units * num_pes_per_unit * node_array_size,
            NULL, &ret);
    printf("Creating explore stack buffer: ");
    print_ret_status(ret);
    cl_mem delete_stack_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
            sizeof(unsigned) * num_compute_units * num_pes_per_unit * node_array_size,
            NULL, &ret);
    printf("Creating delete stack buffer: ");
    print_ret_status(ret);

    // Copy data to buffers
    printf("Copying variables to buffers\n");
    printf("- Initial State Array: ");
    ret = clEnqueueWriteBuffer(command_queue, initial_state_mem_obj, CL_TRUE, 0, 
            sizeof(unsigned) * puzzle_size * puzzle_size, initial_state, 0, NULL, NULL);
    print_ret_status(ret);
    printf("- Final State Array: ");
    ret = clEnqueueWriteBuffer(command_queue, final_state_mem_obj, CL_TRUE, 0, 
            sizeof(unsigned) * puzzle_size * puzzle_size, final_state, 0, NULL, NULL);
    print_ret_status(ret);
    printf("- Puzzle Size: ");
    ret = clEnqueueWriteBuffer(command_queue, puzzle_size_mem_obj, CL_TRUE, 0, 
            sizeof(unsigned), &puzzle_size, 0, NULL, NULL);
    print_ret_status(ret);
    printf("- Blank Position: ");
    ret = clEnqueueWriteBuffer(command_queue, blank_position_mem_obj, CL_TRUE, 0, 
    sizeof(unsigned), &blank_position, 0, NULL, NULL);
    print_ret_status(ret);

    // Create a program from the kernel source
    printf("Creating program from source: ");
    cl_program program = clCreateProgramWithSource(context, 1, 
            (const char **)&source_str, (const size_t *)&source_size, &ret);
    print_ret_status(ret);

    // Build the program
    printf("Building program: ");
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    print_ret_status(ret);
    if(ret != CL_SUCCESS)
    {
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *buildLog = calloc(log_size, sizeof(char));
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, buildLog, NULL);
        buildLog[log_size]='\0';
        printf("\nError in kernel BUILD PGM: %s\n ",buildLog);
        return;
    }

    // Create the OpenCL kernel
    printf("Creating OpenCL kernel: ");
    cl_kernel kernel = clCreateKernel(program, "solve", &ret);
    print_ret_status(ret);

    // Set the arguments of the kernel
    printf("Setting kernel arguments: \n");
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&initial_state_mem_obj);
    printf("1 - "); 
    print_ret_status(ret);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&final_state_mem_obj);
    printf("2 - ");
    print_ret_status(ret);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&puzzle_size_mem_obj);
    printf("3 - ");
    print_ret_status(ret);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&blank_position_mem_obj);
    printf("4 - ");
    print_ret_status(ret);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&CLNodes_mem_obj);
    printf("5 - ");
    print_ret_status(ret);
    ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&explore_stack_mem_obj);
    printf("6 - ");
    print_ret_status(ret);
    ret = clSetKernelArg(kernel, 6, sizeof(cl_mem), (void *)&delete_stack_mem_obj);
    printf("7 - ");
    print_ret_status(ret);

    printf("Executing kernel: \n");
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
                &global_item_size, &local_item_size, 0, NULL, NULL);
    printf("Execution: ");
    print_ret_status(ret);

}

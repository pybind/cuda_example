#include "add.h"

#include <cuda_runtime.h>

__global__ void add_kernel(int i, int j, int *result) { *result = i + j; }

bool cuda_available() {
    int count = 0;
    cudaError_t status = cudaGetDeviceCount(&count);
    return status == cudaSuccess && count > 0;
}

int add(int i, int j) {
    // No device (e.g. running the CUDA-enabled wheel on a machine without a
    // GPU): compute on the CPU so the function still works.
    if (!cuda_available()) {
        return i + j;
    }

    int *d_result = nullptr;
    cudaMalloc(&d_result, sizeof(int));
    add_kernel<<<1, 1>>>(i, j, d_result);

    int result = 0;
    cudaMemcpy(&result, d_result, sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(d_result);
    return result;
}

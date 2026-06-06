#include "add.h"

#include <cuda_runtime.h>

#include <stdexcept>
#include <string>

namespace {

// Throw a Python-friendly exception on any CUDA error.
void check(cudaError_t status) {
    if (status != cudaSuccess) {
        throw std::runtime_error(std::string("CUDA error: ") + cudaGetErrorString(status));
    }
}

}  // namespace

__global__ void add_kernel(int i, int j, int *result) { *result = i + j; }

__global__ void subtract_kernel(int i, int j, int *result) { *result = i - j; }

bool cuda_available() {
    int count = 0;
    cudaError_t status = cudaGetDeviceCount(&count);
    return status == cudaSuccess && count > 0;
}

int add(int i, int j) {
    int *d_result = nullptr;
    check(cudaMalloc(&d_result, sizeof(int)));
    add_kernel<<<1, 1>>>(i, j, d_result);
    check(cudaGetLastError());

    int result = 0;
    check(cudaMemcpy(&result, d_result, sizeof(int), cudaMemcpyDeviceToHost));
    check(cudaFree(d_result));
    return result;
}

int subtract(int i, int j) {
    int *d_result = nullptr;
    check(cudaMalloc(&d_result, sizeof(int)));
    subtract_kernel<<<1, 1>>>(i, j, d_result);
    check(cudaGetLastError());

    int result = 0;
    check(cudaMemcpy(&result, d_result, sizeof(int), cudaMemcpyDeviceToHost));
    check(cudaFree(d_result));
    return result;
}

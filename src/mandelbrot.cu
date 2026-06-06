#include "mandelbrot.h"

#include <cuda_runtime.h>

#include <cstddef>
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

// One CUDA thread computes one pixel. The body matches mandelbrot_cpu() exactly
// so the two are easy to compare.
__global__ void mandelbrot_kernel(MandelbrotParams params, std::int32_t *output) {
    const int col = blockIdx.x * blockDim.x + threadIdx.x;
    const int row = blockIdx.y * blockDim.y + threadIdx.y;
    if (col >= params.width || row >= params.height) {
        return;
    }

    const double dx = (params.xmax - params.xmin) / params.width;
    const double dy = (params.ymax - params.ymin) / params.height;
    const double c_real = params.xmin + col * dx;
    const double c_imag = params.ymin + row * dy;

    double z_real = c_real;
    double z_imag = c_imag;
    int iteration = 0;
    while (iteration < params.max_iterations &&
           z_real * z_real + z_imag * z_imag <= 4.0) {
        const double next_real = z_real * z_real - z_imag * z_imag + c_real;
        z_imag = 2.0 * z_real * z_imag + c_imag;
        z_real = next_real;
        ++iteration;
    }

    output[row * params.width + col] = iteration;
}

bool cuda_available() {
    int count = 0;
    cudaError_t status = cudaGetDeviceCount(&count);
    return status == cudaSuccess && count > 0;
}

void mandelbrot_gpu(const MandelbrotParams &params, std::int32_t *output) {
    const std::size_t pixels = static_cast<std::size_t>(params.width) * params.height;
    const std::size_t bytes = pixels * sizeof(std::int32_t);

    // Allocate device memory, launch a 2D grid of threads, then copy back.
    std::int32_t *d_output = nullptr;
    check(cudaMalloc(&d_output, bytes));

    const dim3 block(16, 16);
    const dim3 grid((params.width + block.x - 1) / block.x,
                    (params.height + block.y - 1) / block.y);
    mandelbrot_kernel<<<grid, block>>>(params, d_output);
    check(cudaGetLastError());

    check(cudaMemcpy(output, d_output, bytes, cudaMemcpyDeviceToHost));
    check(cudaFree(d_output));
}

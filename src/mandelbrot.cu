#include "mandelbrot.h"

#include <cuda_runtime.h>

#include <cstddef>
#include <stdexcept>
#include <string>

// Throw a Python-friendly exception on any CUDA error.
static void check(cudaError_t status) {
    if (status != cudaSuccess) {
        throw std::runtime_error(std::string("CUDA error: ") + cudaGetErrorString(status));
    }
}

// One CUDA thread computes one pixel. The body matches mandelbrot_cpu() exactly
// so the two are easy to compare.
__global__ void mandelbrot_kernel(int width, int height, int max_iterations,
                                  std::int32_t *output) {
    const int col = blockIdx.x * blockDim.x + threadIdx.x;
    const int row = blockIdx.y * blockDim.y + threadIdx.y;
    if (col >= width || row >= height) {
        return;
    }

    // The region of the complex plane to render.
    const double xmin = -2.0, xmax = 1.0;
    const double ymin = -1.5, ymax = 1.5;

    const double c_real = xmin + col * (xmax - xmin) / width;
    const double c_imag = ymin + row * (ymax - ymin) / height;

    double z_real = c_real;
    double z_imag = c_imag;
    int iteration = 0;
    while (iteration < max_iterations && z_real * z_real + z_imag * z_imag <= 4.0) {
        const double next_real = z_real * z_real - z_imag * z_imag + c_real;
        z_imag = 2.0 * z_real * z_imag + c_imag;
        z_real = next_real;
        ++iteration;
    }

    output[row * width + col] = iteration;
}

bool cuda_available() {
    int count = 0;
    cudaError_t status = cudaGetDeviceCount(&count);
    return status == cudaSuccess && count > 0;
}

void mandelbrot_gpu(int width, int height, int max_iterations, std::int32_t *output) {
    const std::size_t bytes = static_cast<std::size_t>(width) * height * sizeof(std::int32_t);

    // Allocate device memory, launch a 2D grid of threads, then copy back.
    std::int32_t *device_output = nullptr;
    check(cudaMalloc(&device_output, bytes));

    const dim3 block(16, 16);
    const dim3 grid((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);
    mandelbrot_kernel<<<grid, block>>>(width, height, max_iterations, device_output);
    check(cudaGetLastError());

    check(cudaMemcpy(output, device_output, bytes, cudaMemcpyDeviceToHost));
    check(cudaFree(device_output));
}

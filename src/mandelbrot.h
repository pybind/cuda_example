#pragma once

#include <cstdint>

// Each function fills `output`, a row-major height x width buffer, with the
// escape iteration count for every pixel. The CPU and GPU versions are written
// the same way so they are easy to compare.

// Compute the Mandelbrot set on the CPU.
void mandelbrot_cpu(int width, int height, int max_iterations, std::int32_t *output);

// Compute the Mandelbrot set on the GPU with CUDA. Throws std::runtime_error if
// no CUDA device is available.
void mandelbrot_gpu(int width, int height, int max_iterations, std::int32_t *output);

// Returns true if a CUDA-capable device is present at runtime.
bool cuda_available();

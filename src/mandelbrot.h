#pragma once

#include <cstdint>

// Parameters describing the image to render and the region of the complex
// plane it covers. Kept as a small plain struct so it can be passed by value to
// the CPU function and copied into a CUDA kernel.
struct MandelbrotParams {
    int width;
    int height;
    int max_iterations;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

// Each of these fills `output` (a row-major height x width buffer) with the
// escape iteration count for every pixel. The two implementations are
// deliberately written the same way so they are easy to compare.

// Compute the Mandelbrot set on the CPU.
void mandelbrot_cpu(const MandelbrotParams &params, std::int32_t *output);

// Compute the Mandelbrot set on the GPU with CUDA. Throws std::runtime_error if
// no CUDA device is available.
void mandelbrot_gpu(const MandelbrotParams &params, std::int32_t *output);

// Returns true if a CUDA-capable device is present at runtime.
bool cuda_available();

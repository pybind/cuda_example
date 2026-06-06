#pragma once

// Returns true if a CUDA-capable device is present at runtime.
bool cuda_available();

// Adds two integers on the GPU with a CUDA kernel. Throws std::runtime_error if
// no CUDA device is available.
int add(int i, int j);

// Subtracts two integers on the GPU with a CUDA kernel. Throws
// std::runtime_error if no CUDA device is available.
int subtract(int i, int j);

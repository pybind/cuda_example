#pragma once

// Returns true if a CUDA-capable device is present at runtime. Always false
// when the extension was built without the CUDA Toolkit (CPU fallback).
bool cuda_available();

// Adds two integers. Uses a CUDA kernel when a device is available, otherwise
// falls back to a plain CPU addition so the extension is usable everywhere.
int add(int i, int j);

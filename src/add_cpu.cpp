#include "add.h"

// CPU fallback used when the CUDA Toolkit is not available at build time. This
// keeps the example buildable on platforms without CUDA (macOS, Windows without
// the toolkit, PyPy/Pyodide wheels, ...).

bool cuda_available() { return false; }

int add(int i, int j) { return i + j; }

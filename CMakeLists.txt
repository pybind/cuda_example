# Require CMake 3.18+ (matching scikit-build-core and supporting modern CUDA).
# Use new versions of all policies up to CMake 4.0
cmake_minimum_required(VERSION 3.18...4.0)

# Scikit-build-core sets these values for you, or you can just hard-code the
# name and version.
project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

# Find pybind11 and Python
find_package(pybind11 CONFIG REQUIRED)

# Enable CUDA if a compiler (nvcc) is available. This lets the example build
# everywhere: with the CUDA Toolkit it compiles the real kernel, and without it
# it falls back to a CPU implementation.
include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
  message(STATUS "CUDA found: building the GPU implementation")
  set(ADD_SOURCE src/add.cu)
else()
  message(STATUS "CUDA not found: building the CPU fallback")
  set(ADD_SOURCE src/add_cpu.cpp)
endif()

# Add a library using FindPython's tooling (pybind11 also provides a helper like
# this)
python_add_library(_core MODULE src/main.cpp ${ADD_SOURCE} WITH_SOABI)
target_link_libraries(_core PRIVATE pybind11::headers)

# This is passing in the version as a define just as an example
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

if(CMAKE_CUDA_COMPILER)
  # Build for all major architectures so the wheel is portable, and link the
  # CUDA runtime statically so the wheel does not depend on libcudart.
  if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set_property(TARGET _core PROPERTY CUDA_ARCHITECTURES all-major)
  endif()
  set_property(TARGET _core PROPERTY CUDA_RUNTIME_LIBRARY Static)
  target_compile_definitions(_core PRIVATE WITH_CUDA)
endif()

# The install directory is the output (wheel) directory
install(TARGETS _core DESTINATION cuda_example)

# Require CMake 3.18+ (matching scikit-build-core and supporting modern CUDA).
# Use new versions of all policies up to CMake 4.0
cmake_minimum_required(VERSION 3.18...4.0)

# Scikit-build-core sets these values for you, or you can just hard-code the
# name and version. CUDA is a required language: configuration fails without a
# CUDA compiler (nvcc).
project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX CUDA)

# Find pybind11 and Python
find_package(pybind11 CONFIG REQUIRED)

# Add a library using FindPython's tooling (pybind11 also provides a helper like
# this), combining the pybind11 bindings with the CUDA kernels.
python_add_library(_core MODULE src/main.cpp src/add.cu WITH_SOABI)
target_link_libraries(_core PRIVATE pybind11::headers)

# This is passing in the version as a define just as an example
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

# Build for all major architectures so the wheel is portable, and link the CUDA
# runtime statically so the wheel does not depend on libcudart.
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
  set_property(TARGET _core PROPERTY CUDA_ARCHITECTURES all-major)
endif()
set_property(TARGET _core PROPERTY CUDA_RUNTIME_LIBRARY Static)

# The install directory is the output (wheel) directory
install(TARGETS _core DESTINATION cuda_example)

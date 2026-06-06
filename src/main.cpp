#include <pybind11/pybind11.h>

#include "add.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(_core, m, py::mod_gil_not_used(), py::multiple_interpreters::per_interpreter_gil()) {
    m.doc() = R"pbdoc(
        Pybind11 + CUDA example plugin
        ------------------------------

        .. currentmodule:: cuda_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
           cuda_available
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers on the GPU with a CUDA kernel.
    )pbdoc");

    m.def("subtract", &subtract, R"pbdoc(
        Subtract two numbers on the GPU with a CUDA kernel.
    )pbdoc");

    m.def("cuda_available", &cuda_available, R"pbdoc(
        Return True if a CUDA-capable device is available at runtime.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

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
        Add two numbers

        Runs on the GPU with CUDA when a device is available, otherwise falls
        back to the CPU.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    m.def("cuda_available", &cuda_available, R"pbdoc(
        Return True if a CUDA-capable device is available at runtime.
    )pbdoc");

    // True if the extension was compiled with the CUDA Toolkit, regardless of
    // whether a device is present at runtime.
#ifdef WITH_CUDA
    m.attr("WITH_CUDA") = true;
#else
    m.attr("WITH_CUDA") = false;
#endif

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

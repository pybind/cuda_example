#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <cstdint>

#include "mandelbrot.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(_core, m, py::mod_gil_not_used(), py::multiple_interpreters::per_interpreter_gil()) {
    m.doc() = R"pbdoc(
        Pybind11 + CUDA Mandelbrot example
        ----------------------------------

        .. currentmodule:: cuda_example

        .. autosummary::
           :toctree: _generate

           mandelbrot_cpu
           mandelbrot_gpu
           cuda_available
    )pbdoc";

    m.def(
        "mandelbrot_cpu",
        [](int width, int height, int max_iterations) {
            // Allocate the (height, width) output image and fill it on the CPU,
            // releasing the GIL while the C++ code runs.
            py::array_t<std::int32_t> image({height, width});
            std::int32_t *data = image.mutable_data();
            {
                py::gil_scoped_release release;
                mandelbrot_cpu(width, height, max_iterations, data);
            }
            return image;
        },
        py::arg("width") = 800, py::arg("height") = 600, py::arg("max_iterations") = 100,
        "Render the Mandelbrot set on the CPU, returning a (height, width) int32 array.");

    m.def(
        "mandelbrot_gpu",
        [](int width, int height, int max_iterations) {
            py::array_t<std::int32_t> image({height, width});
            std::int32_t *data = image.mutable_data();
            {
                py::gil_scoped_release release;
                mandelbrot_gpu(width, height, max_iterations, data);
            }
            return image;
        },
        py::arg("width") = 800, py::arg("height") = 600, py::arg("max_iterations") = 100,
        "Render the Mandelbrot set on the GPU, returning a (height, width) int32 array.");

    m.def("cuda_available", &cuda_available,
          "Return True if a CUDA-capable device is available at runtime.");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <cstdint>
#include <stdexcept>

#include "mandelbrot.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

namespace {

using Image = py::array_t<std::int32_t, py::array::c_style>;

// Shared wrapper: validate the arguments, allocate the output image, and run one
// of the compute functions with the GIL released. Returns a (height, width)
// NumPy array of escape counts.
Image render(void (*compute)(const MandelbrotParams &, std::int32_t *), int width, int height,
             int max_iterations, double xmin, double xmax, double ymin, double ymax) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("width and height must be positive");
    }
    if (max_iterations <= 0) {
        throw std::invalid_argument("max_iterations must be positive");
    }

    const MandelbrotParams params{width, height, max_iterations, xmin, xmax, ymin, ymax};
    Image image({height, width});
    std::int32_t *data = image.mutable_data();

    {
        py::gil_scoped_release release;
        compute(params, data);
    }
    return image;
}

}  // namespace

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

    const char *doc = R"pbdoc(
        Render the Mandelbrot set.

        Returns a ``(height, width)`` int32 NumPy array; each value is the number
        of iterations before the point escaped (``max_iterations`` if it never
        did).
    )pbdoc";

    m.def("mandelbrot_cpu",
          [](int width, int height, int max_iterations, double xmin, double xmax, double ymin,
             double ymax) {
              return render(&mandelbrot_cpu, width, height, max_iterations, xmin, xmax, ymin, ymax);
          },
          py::arg("width") = 800, py::arg("height") = 600, py::arg("max_iterations") = 100,
          py::arg("xmin") = -2.0, py::arg("xmax") = 1.0, py::arg("ymin") = -1.25,
          py::arg("ymax") = 1.25, doc);

    m.def("mandelbrot_gpu",
          [](int width, int height, int max_iterations, double xmin, double xmax, double ymin,
             double ymax) {
              return render(&mandelbrot_gpu, width, height, max_iterations, xmin, xmax, ymin, ymax);
          },
          py::arg("width") = 800, py::arg("height") = 600, py::arg("max_iterations") = 100,
          py::arg("xmin") = -2.0, py::arg("xmax") = 1.0, py::arg("ymin") = -1.25,
          py::arg("ymax") = 1.25, doc);

    m.def("cuda_available", &cuda_available, R"pbdoc(
        Return True if a CUDA-capable device is available at runtime.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}

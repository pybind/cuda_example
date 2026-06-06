#include "mandelbrot.h"

// CPU reference implementation. For every pixel we iterate z = z^2 + c starting
// from z = c, and record how many iterations it takes for |z| to exceed 2
// (i.e. |z|^2 > 4). Points that never escape get `max_iterations`.

void mandelbrot_cpu(const MandelbrotParams &params, std::int32_t *output) {
    const double dx = (params.xmax - params.xmin) / params.width;
    const double dy = (params.ymax - params.ymin) / params.height;

    for (int row = 0; row < params.height; ++row) {
        const double c_imag = params.ymin + row * dy;

        for (int col = 0; col < params.width; ++col) {
            const double c_real = params.xmin + col * dx;

            double z_real = c_real;
            double z_imag = c_imag;
            int iteration = 0;
            while (iteration < params.max_iterations &&
                   z_real * z_real + z_imag * z_imag <= 4.0) {
                const double next_real = z_real * z_real - z_imag * z_imag + c_real;
                z_imag = 2.0 * z_real * z_imag + c_imag;
                z_real = next_real;
                ++iteration;
            }

            output[row * params.width + col] = iteration;
        }
    }
}

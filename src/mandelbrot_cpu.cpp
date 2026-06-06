#include "mandelbrot.h"

// CPU implementation. For every pixel we iterate z = z^2 + c starting from
// z = c, and record how many steps it takes for |z| to exceed 2 (i.e.
// |z|^2 > 4). Points that never escape get `max_iterations`.

void mandelbrot_cpu(int width, int height, int max_iterations, std::int32_t *output) {
    // The region of the complex plane to render.
    const double xmin = -2.0, xmax = 1.0;
    const double ymin = -1.5, ymax = 1.5;

    for (int row = 0; row < height; ++row) {
        const double c_imag = ymin + row * (ymax - ymin) / height;

        for (int col = 0; col < width; ++col) {
            const double c_real = xmin + col * (xmax - xmin) / width;

            double z_real = c_real;
            double z_imag = c_imag;
            int iteration = 0;
            while (iteration < max_iterations && z_real * z_real + z_imag * z_imag <= 4.0) {
                const double next_real = z_real * z_real - z_imag * z_imag + c_real;
                z_imag = 2.0 * z_real * z_imag + c_imag;
                z_real = next_real;
                ++iteration;
            }

            output[row * width + col] = iteration;
        }
    }
}

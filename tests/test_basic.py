from __future__ import annotations

import numpy as np
import pytest

import cuda_example as m

# mandelbrot_gpu launches a CUDA kernel, so it can only run on a machine with a
# CUDA device. Skip (rather than fail) when no GPU is available, e.g. in CI.
requires_cuda = pytest.mark.skipif(
    not m.cuda_available(), reason="no CUDA device available"
)


def test_version():
    assert m.__version__ == "0.0.1"


def test_cpu_shape_and_dtype():
    image = m.mandelbrot_cpu(width=40, height=30, max_iterations=50)
    assert image.shape == (30, 40)
    assert image.dtype == np.int32
    # The interior of the set (e.g. the origin) never escapes.
    assert image.max() == 50


@requires_cuda
def test_gpu_matches_cpu():
    kwargs = {"width": 128, "height": 96, "max_iterations": 100}
    assert np.array_equal(m.mandelbrot_cpu(**kwargs), m.mandelbrot_gpu(**kwargs))

"""
Pybind11 + CUDA Mandelbrot example
----------------------------------

.. currentmodule:: cuda_example

.. autosummary::
    :toctree: _generate

    mandelbrot_cpu
    mandelbrot_gpu
    cuda_available
"""

from __future__ import annotations

from numpy import int32
from numpy.typing import NDArray

def mandelbrot_cpu(
    width: int = ...,
    height: int = ...,
    max_iterations: int = ...,
    xmin: float = ...,
    xmax: float = ...,
    ymin: float = ...,
    ymax: float = ...,
) -> NDArray[int32]:
    """
    Render the Mandelbrot set on the CPU.

    Returns a ``(height, width)`` int32 array of escape counts.
    """

def mandelbrot_gpu(
    width: int = ...,
    height: int = ...,
    max_iterations: int = ...,
    xmin: float = ...,
    xmax: float = ...,
    ymin: float = ...,
    ymax: float = ...,
) -> NDArray[int32]:
    """
    Render the Mandelbrot set on the GPU with CUDA.

    Returns a ``(height, width)`` int32 array of escape counts.
    """

def cuda_available() -> bool:
    """
    Return True if a CUDA-capable device is available at runtime.
    """

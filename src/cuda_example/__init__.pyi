"""
Pybind11 + CUDA example plugin
------------------------------

.. currentmodule:: cuda_example

.. autosummary::
    :toctree: _generate

    add
    subtract
    cuda_available
"""

from __future__ import annotations

def add(i: int, j: int) -> int:
    """
    Add two numbers on the GPU with a CUDA kernel.
    """

def subtract(i: int, j: int) -> int:
    """
    Subtract two numbers on the GPU with a CUDA kernel.
    """

def cuda_available() -> bool:
    """
    Return True if a CUDA-capable device is available at runtime.
    """

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

WITH_CUDA: bool

def add(i: int, j: int) -> int:
    """
    Add two numbers

    Runs on the GPU with CUDA when a device is available, otherwise falls
    back to the CPU.
    """

def subtract(i: int, j: int) -> int:
    """
    Subtract two numbers

    Some other explanation about the subtract function.
    """

def cuda_available() -> bool:
    """
    Return True if a CUDA-capable device is available at runtime.
    """

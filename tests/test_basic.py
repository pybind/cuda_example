from __future__ import annotations

import pytest

import cuda_example as m

# add/subtract launch CUDA kernels, so they can only run on a machine with a
# CUDA device. Skip them (rather than fail) when no GPU is available, e.g. in CI.
requires_cuda = pytest.mark.skipif(
    not m.cuda_available(), reason="no CUDA device available"
)


def test_version():
    assert m.__version__ == "0.0.1"


@requires_cuda
def test_add():
    assert m.add(1, 2) == 3


@requires_cuda
def test_sub():
    assert m.subtract(1, 2) == -1

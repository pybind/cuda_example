from __future__ import annotations

import cuda_example as m


def test_version():
    assert m.__version__ == "0.0.1"


def test_add():
    assert m.add(1, 2) == 3


def test_sub():
    assert m.subtract(1, 2) == -1


def test_cuda_available():
    # cuda_available() reflects whether a device is present at runtime; it must
    # be False unless the wheel was built with CUDA and a GPU is visible.
    available = m.cuda_available()
    assert isinstance(available, bool)
    assert not available or m.WITH_CUDA

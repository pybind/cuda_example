# cuda_example

[![Gitter][gitter-badge]][gitter-link]

|      CI              | status |
|----------------------|--------|
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |
| wheels               | [![Wheels Actions Status][actions-wheels-badge]][actions-wheels-link] |


An example project built with [pybind11][], [CUDA][], and
[scikit-build-core][]. Python 3.9+.

The extension renders the [Mandelbrot set][mandelbrot] two ways — once on the
CPU and once on the GPU — so you can read both side by side and compare their
performance. The two implementations are written the same way on purpose:

* `src/mandelbrot_cpu.cpp` — a plain nested loop over every pixel
* `src/mandelbrot.cu` — the same logic as a CUDA kernel, one thread per pixel

Both return a `(height, width)` int32 NumPy array of escape counts. Building
**requires** the CUDA Toolkit (`nvcc`): the CMake project declares CUDA as a
required language, so configuration fails without it. The CUDA runtime is linked
statically, so the resulting wheels do not depend on `libcudart` and stay
importable on machines without a GPU — calling `mandelbrot_gpu` there raises, but
`cuda_available()` lets you check first.


[gitter-badge]:            https://badges.gitter.im/pybind/Lobby.svg
[gitter-link]:             https://gitter.im/pybind/Lobby
[actions-pip-link]:        https://github.com/pybind/cuda_example/actions?query=workflow%3APip
[actions-pip-badge]:       https://github.com/pybind/cuda_example/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/pybind/cuda_example/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/pybind/cuda_example/workflows/Wheels/badge.svg

## Installation

- Clone this repository
- `pip install ./cuda_example`

The CUDA Toolkit (`nvcc`) must be installed and discoverable by CMake.

## Test call

```python
import cuda_example

# (height, width) int32 array of escape counts
image = cuda_example.mandelbrot_cpu(width=800, height=600, max_iterations=100)

if cuda_example.cuda_available():
    image = cuda_example.mandelbrot_gpu(width=800, height=600, max_iterations=100)
```

You can view the result with any plotting library, e.g.:

```python
import matplotlib.pyplot as plt

plt.imshow(image, extent=(-2, 1, -1.25, 1.25), cmap="twilight_shifted")
plt.show()
```

## Comparing CPU and GPU

Because both functions take the same arguments and return identical arrays, you
can run them back to back and time them (on a machine with a GPU):

```python
import time
import cuda_example

size = {"width": 2000, "height": 1500, "max_iterations": 200}

start = time.perf_counter()
cpu = cuda_example.mandelbrot_cpu(**size)
print(f"CPU: {time.perf_counter() - start:.3f}s")

start = time.perf_counter()
gpu = cuda_example.mandelbrot_gpu(**size)
print(f"GPU: {time.perf_counter() - start:.3f}s")

assert (cpu == gpu).all()  # identical results, very different runtimes
```

## Building CUDA wheels

The `Wheels` workflow builds CUDA-enabled Linux wheels with [cibuildwheel][] by
pointing it at the custom manylinux images that ship the CUDA Toolkit (see
[pypa/cibuildwheel#2896][cibw-cuda]):

```yaml
- uses: pypa/cibuildwheel@v3.4
  env:
    CIBW_MANYLINUX_X86_64_IMAGE: quay.io/manylinux_cuda/manylinux_2_28_x86_64_cuda13_1:latest
    CIBW_MANYLINUX_AARCH64_IMAGE: quay.io/manylinux_cuda/manylinux_2_28_aarch64_cuda13_1:latest
```

The CUDA runtime is linked statically (`CUDA_RUNTIME_LIBRARY Static`), so the
resulting wheels do not depend on `libcudart`. GitHub-hosted runners have no
GPU, so the wheels are compiled and imported, but the kernels themselves only
run on a machine with a CUDA device.

## Testing the CUDA build locally with Docker

You don't need a GPU (or even a Linux machine) to compile and import the CUDA
build — the manylinux images ship the CUDA Toolkit, so `nvcc` runs inside the
container. The kernels are *compiled* and the wheel is *imported*; they just
can't *execute* on the GPU without a device (those tests are skipped).

Pick the image matching your host architecture (the `aarch64` image runs
natively on Apple Silicon; on x86_64 use the `x86_64` image):

```bash
# Apple Silicon / arm64 host:
IMAGE=quay.io/manylinux_cuda/manylinux_2_28_aarch64_cuda13_1:latest
# x86_64 host:
# IMAGE=quay.io/manylinux_cuda/manylinux_2_28_x86_64_cuda13_1:latest

mkdir -p wheelhouse
docker run --rm \
  -v "$PWD":/io:ro \
  -v "$PWD/wheelhouse":/wheelhouse \
  "$IMAGE" bash -lc '
    PY=/opt/python/cp312-cp312/bin/python
    cp -r /io /tmp/src && cd /tmp/src
    $PY -m pip install --upgrade pip build pytest
    $PY -m build --wheel --outdir /wheelhouse .   # compiles src/mandelbrot.cu with nvcc
    $PY -m pip install /wheelhouse/*.whl
    $PY -m pytest                                  # GPU tests skip (no device)
  '
```

The compiled wheel is written to `./wheelhouse/` on the host, so you can inspect
or install it afterwards. Because the container has no GPU, `cuda_available()`
returns `False` and the `mandelbrot_gpu` test is skipped (the `mandelbrot_cpu`
tests still run). The same flow runs in CI in the `cuda` job of
`.github/workflows/pip.yml`.

## Files

This example has several files that are a good idea, but aren't strictly
necessary. The necessary files are:

* `pyproject.toml`: The Python project file
* `CMakeLists.txt`: The CMake configuration file, which requires the CUDA language
* `src/main.cpp`: The pybind11 bindings (turns the results into NumPy arrays)
* `src/mandelbrot_cpu.cpp`: The CPU implementation
* `src/mandelbrot.cu`: The CUDA kernel and runtime device query
* `src/mandelbrot.h`: The shared declarations
* `src/cuda_example/__init__.py`: The Python portion of the module. The root of the module needs to be `<package_name>`, `src/<package_name>`, or `python/<package_name>` to be auto-discovered.

These files are also expected and highly recommended:

* `.gitignore`: Git's ignore list, also used by `scikit-build-core` to select files for the SDist
* `README.md`: The source for the PyPI description
* `LICENSE`: The license file

There are also several completely optional directories:

* `.github`: configuration for [Dependabot][] and [GitHub Actions][]
* `docs/`: Documentation
* `tests/`: Tests go here

And some optional files:

* `.pre-commit-config.yaml`: Configuration for the fantastic static-check runner [pre-commit][].
* `noxfile.py`: Configuration for the [nox][] task runner, which helps make setup easier for contributors.

This is a simplified version of the recommendations in the [Scientific-Python
Development Guide][], which is a _highly_ recommended read for anyone
interested in Python package development (Scientific or not). The guide also
has a cookiecutter that includes scikit-build-core and pybind11 as a backend
choice.

### CI Examples

There are examples for CI in `.github/workflows`. The "wheels.yml" file builds
CUDA-enabled binary "wheels" for Linux (x86_64 and aarch64) using
[cibuildwheel][], and "pip.yml" does a quick build-and-import check in the CUDA
containers.

## License

pybind11 is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

[cibuildwheel]: https://cibuildwheel.readthedocs.io
[cibw-cuda]: https://github.com/pypa/cibuildwheel/pull/2896
[cuda]: https://developer.nvidia.com/cuda-toolkit
[mandelbrot]: https://en.wikipedia.org/wiki/Mandelbrot_set
[scientific-python development guide]: https://learn.scientific-python.org/development
[dependabot]: https://docs.github.com/en/code-security/dependabot
[github actions]: https://docs.github.com/en/actions
[pre-commit]: https://pre-commit.com
[nox]: https://nox.thea.codes
[pybind11]: https://pybind11.readthedocs.io
[scikit-build-core]: https://scikit-build-core.readthedocs.io

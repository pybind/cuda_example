# cuda_example

[![Gitter][gitter-badge]][gitter-link]

|      CI              | status |
|----------------------|--------|
| conda.recipe         | [![Conda Actions Status][actions-conda-badge]][actions-conda-link] |
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |


An example project built with [pybind11][], [CUDA][], and
[scikit-build-core][]. Python 3.9+.

The extension exposes a tiny `add` function that runs on the GPU with a CUDA
kernel when a device is available, and otherwise falls back to the CPU. The
build is conditional: if the CUDA Toolkit (`nvcc`) is found at configure time,
the real kernel in `src/add.cu` is compiled; otherwise the CPU implementation in
`src/add_cpu.cpp` is used. This keeps the example buildable everywhere (macOS,
Windows, PyPy, Pyodide, ...) while still demonstrating a real CUDA build on
Linux.


[gitter-badge]:            https://badges.gitter.im/pybind/Lobby.svg
[gitter-link]:             https://gitter.im/pybind/Lobby
[actions-badge]:           https://github.com/pybind/cuda_example/workflows/Tests/badge.svg
[actions-conda-link]:      https://github.com/pybind/cuda_example/actions?query=workflow%3AConda
[actions-conda-badge]:     https://github.com/pybind/cuda_example/workflows/Conda/badge.svg
[actions-pip-link]:        https://github.com/pybind/cuda_example/actions?query=workflow%3APip
[actions-pip-badge]:       https://github.com/pybind/cuda_example/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/pybind/cuda_example/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/pybind/cuda_example/workflows/Wheels/badge.svg

## Installation

- Clone this repository
- `pip install ./cuda_example`

If the CUDA Toolkit is installed, the GPU implementation is built automatically.

## Test call

```python
import cuda_example

cuda_example.add(1, 2)       # 3 (on the GPU if one is available)
cuda_example.cuda_available()  # True if a CUDA device is visible at runtime
cuda_example.WITH_CUDA         # True if the wheel was compiled with CUDA
```

## Building CUDA wheels

The default `Wheels` workflow builds the CPU fallback on every platform using
[cibuildwheel][]. To build CUDA-enabled Linux wheels, the
`.github/workflows/cibw-cuda.yaml` workflow points cibuildwheel at the custom
manylinux images that ship the CUDA Toolkit (see
[pypa/cibuildwheel#2896][cibw-cuda]):

```yaml
- uses: pypa/cibuildwheel@v3.4
  env:
    CIBW_MANYLINUX_X86_64_IMAGE: quay.io/manylinux_cuda/manylinux_2_28_x86_64_cuda13_1:latest
    CIBW_MANYLINUX_AARCH64_IMAGE: quay.io/manylinux_cuda/manylinux_2_28_aarch64_cuda13_1:latest
```

The CUDA runtime is linked statically (`CUDA_RUNTIME_LIBRARY Static`), so the
resulting wheels do not depend on `libcudart` and remain importable on machines
without a GPU (where `add` transparently falls back to the CPU). GitHub-hosted
runners have no GPU, so the wheels are compiled and imported, but the kernel
itself only runs on a machine with a CUDA device.

## Files

This example has several files that are a good idea, but aren't strictly
necessary. The necessary files are:

* `pyproject.toml`: The Python project file
* `CMakeLists.txt`: The CMake configuration file, which conditionally enables CUDA
* `src/main.cpp`: The pybind11 bindings
* `src/add.cu`: The CUDA kernel implementation (built when CUDA is available)
* `src/add_cpu.cpp`: The CPU fallback (built when CUDA is not available)
* `src/add.h`: The shared declarations
* `src/cuda_example/__init__.py`: The Python portion of the module. The root of the module needs to be `<package_name>`, `src/<package_name>`, or `python/<package_name>` to be auto-discovered.

These files are also expected and highly recommended:

* `.gitignore`: Git's ignore list, also used by `scikit-build-core` to select files for the SDist
* `README.md`: The source for the PyPI description
* `LICENSE`: The license file

There are also several completely optional directories:

* `.github`: configuration for [Dependabot][] and [GitHub Actions][]
* `conda.recipe`: Example recipe. Normally you should submit projects to conda-forge instead of building them yourself, but this is useful for testing the example.
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

There are examples for CI in `.github/workflows`. A simple way to produce
binary "wheels" for all platforms is illustrated in the "wheels.yml" file,
using [cibuildwheel][]. The "cibw-cuda.yaml" file shows how to build
CUDA-enabled wheels on Linux.

## License

pybind11 is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

[cibuildwheel]: https://cibuildwheel.readthedocs.io
[cibw-cuda]: https://github.com/pypa/cibuildwheel/pull/2896
[cuda]: https://developer.nvidia.com/cuda-toolkit
[scientific-python development guide]: https://learn.scientific-python.org/development
[dependabot]: https://docs.github.com/en/code-security/dependabot
[github actions]: https://docs.github.com/en/actions
[pre-commit]: https://pre-commit.com
[nox]: https://nox.thea.codes
[pybind11]: https://pybind11.readthedocs.io
[scikit-build-core]: https://scikit-build-core.readthedocs.io

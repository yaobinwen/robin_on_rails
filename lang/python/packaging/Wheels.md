# Wheels

## General Info

This section mainly refers to [3].

Python has two types of distributions:
- `source distribution` (`sdist`):
  - "provides metadata and the essential source files needed for installing" [2];
  - requires an additional building step to generate a `built distribution` before it can be installed [2];
  - has an accompanying `.egg-info` directory for the metadata;
  - is created by `python3 setup.py sdist`.
- `built distribution`: (`bdist`):
  - contains "... files and metadata that only need to be moved to the correct location on the target system, to be installed" [2];

`Egg` is a built distribution format introduced by `setuptools`.

`Wheel` is a built distribution format that is intended to replace the `Egg` format. It can be created by `python3 setup.py bdist_wheel`.

A `wheel`, or a `.whl` file, is essentially a `.zip` archive.

A `wheel` doesn't contain `.pyc` files.

A `wheel` file name consists of the following elements: `{dist}-{version}(-{build})?-{python}-{abi}-{platform}.whl`

You can find many wheels on [4]: [Python Wheels](https://pythonwheels.com/)

## pip

`pip` prefers the wheel format **if** it's compatible with your operating system.
  - which means if a Python package doesn't provide a wheel that fits your operating system, `pip` won't install it.

`pip` has two options:
- `--only-binary`: install through wheel distribution only.
- `--no-binary`: install through source distribution only.

## `manylinux`

`manylinux` is essentially a Docker image "built off a certain version of the CentOS operating system". [3] Note these wheels bundle their binary dependencies inside so these binary dependencies cannot be updated for, say, security reasons, by the host system's package manager. For example [3]:

> For example, if a wheel incorporates the libfortran shared library, then distributions of that wheel will use the libfortran version that they were bundled with even if you upgrade your own machine's version of libfortran with a package manager such as apt, yum, or brew.

[5] is a project to advocate not to use `manylinux`.

## Building Wheels

Install `wheel` in order to build wheels: `python3 -m pip install -U wheel setuptools`
- This command also ensures `setuptools` is up to date.
- I guess `setuptools` needs the module `wheel` in order to call the sub-command `bdist_wheel`. See below.

There are three types of wheels:
- `universal`:
  - `py2.py3-none-any.whl`;
  - supports **both** Python 2 and Python 3 and **all** OS platforms.
- `pure-Python`:
  - either `py3-none-any.whl` or `py2-none-any.whl`;
- `platform`:
  - supports a specific Python version and platform;
  - usually "contains an extension module or depends on a library written in a statically typed language such as C, C++" [3]. (Because these dependencies must be built on a per-platform basis, their dependent Python package must be platform-specific, too.)

How to build a `pure-Python` wheel: `python3 setup.py bdist_wheel`. The result is in the generated sub-directory `dist`.

How to build a `universal` wheel: `python setup.py bdist_wheel --universal`.
- Or, specify `--universal` in the project's [`setup.cfg` file](https://docs.python.org/3/distutils/configfile.html).
- Or, specify `--universal` in `setup.py` (least recommended):

```python
# setup.py
from setuptools import setup

setup(
    # ....
    options={"bdist_wheel": {"universal": True}}
    # ....
)
```

How to build a `platform` wheel: refer to the related section in [3].

## Building Wheels in CI Pipeline

Refer to [Building Wheels in Continuous Integration](https://realpython.com/python-wheels/#building-wheels-in-continuous-integration) in [3] for more information.

## Checking Wheel Contents

Two options:
- Use [`check-wheel-contents`](https://github.com/jwodder/check-wheel-contents).
- Use [`TestPyPI`](https://packaging.python.org/guides/using-testpypi/): upload the wheel to TestPyPI and download to see if it works as expected.

## References

- [1] [Python Packaging User Guide](https://packaging.python.org/)
- [2] [Python Packaging Glossary](https://packaging.python.org/glossary/)
- [3] [What Are Python Wheels and Why Should You Care?](https://realpython.com/python-wheels/)
- [4] [Python Wheels](https://pythonwheels.com/)
- [5] [no-manylinux](https://github.com/asottile/no-manylinux)

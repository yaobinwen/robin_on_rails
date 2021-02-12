# Python Packaging

## Overview

This document has the notes about Python packaging. [1] is the official user guide for this topic. As of 2021-01-06, [1] is organized with the following sections:

- `Requirements for packaging and distributing`: This section lists the tools that are needed for packaging and distributing.
- `Configuring your project`: This section talks about the minimal configuration that should be done for the project in order to be properly packaged and distributed.
  - This section then talks about the commonly used arguments in the file `setup.py` which is the key part of the configuration.
    - Among the commonly used arguments, `version` needs some attention, so this section talks about it further.
- `Working in "development mode"`: This section briefly talks about the support for developers when the package is still being developed.
- `Packaging your project`: This section talks about the two forms of distribution: `source` and `wheel`.
- `Uploading your Project to PyPI`: This section talks about how to publish a package.

## Wheels

See [`Wheels.md`](./Wheels.md).

## tox

`tox` aims to "automate and standardize testing in Python. It is part of a larger vision of easing the packaging, testing and release process of Python software". [3]

## Python Package Index

There are several options to host your own Python package server:

- [4] mentions two options:
  - [`devpi`](https://devpi.net/docs/devpi/devpi/latest/%2Bd/index.html) (which looks intimidating on the first glance but it's actually easy to use)
  - "Manual" repository
- [`Nexus` for PyPI repository](https://help.sonatype.com/repomanager3/formats/pypi-repositories).
- [`pypiserver`](https://pypi.org/project/pypiserver/) looks really neat!

## `requirements.txt`

The process to determine `requirements.txt` is as follows:

1. Examine every `import` statement in the app.
2. Figure out what Python distribution(s), if any, are required to make that `import` statement work. (Some `import` statements refer to built-in modules of Python and so we don't need to add a distribution to requirements.txt to make them work.)
3. To make the `control` file, go find the "egg info" in `/usr/lib/python3/dist-packages` that corresponds to each distribution. Do `dpkg-query -S /path/to/egg/info`. This tells you the Debian package name that matches that distribution.
4. Test the build in the official build system to find special cases like "`gridfs` is not installed by default with `pymongo` .deb package" which we have no way of knowing a priori.

Note that if a package is only required for the build (e.g., only required for `setup.py`) and not actually to install/run the program, then it should be listed in the control file but not in `requirements.txt`.

## References

- [1] [Python Packaging User Guide](https://packaging.python.org/)
- [2] [Python Packaging Glossary](https://packaging.python.org/glossary/)
- [3] [tox](https://tox.readthedocs.io/en/latest/)
- [4] [Hosting your own simple repository](https://packaging.python.org/guides/hosting-your-own-index/)

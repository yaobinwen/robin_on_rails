# Python Packaging

## Overview

This document has the notes about Python packaging. [1] is the official user guide for this topic.

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

## References

- [1] [Python Packaging User Guide](https://packaging.python.org/)
- [2] [Python Packaging Glossary](https://packaging.python.org/glossary/)
- [3] [tox](https://tox.readthedocs.io/en/latest/)
- [4] [Hosting your own simple repository](https://packaging.python.org/guides/hosting-your-own-index/)

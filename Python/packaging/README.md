# Python Packaging

## 1. Overview

This document has the notes about Python packaging. [1] is the official user guide for this topic.

As of 2021-07-17, I do recommend to ignore the main page of [1] but take a look at the list on the left panel which lists the topics in a more reasonable order. Click each item to see the details and also expand that item to see the sub-topics.

The GitHub organization [`pypa`](https://github.com/pypa) is "Python Packaging Authority " which holds many repositories of the packaging tools and documents such as:

- [setuptools](https://github.com/pypa/setuptools)
- [pipenv](https://github.com/pypa/pipenv)
- [twine](https://github.com/pypa/twine)

More importantly, `pypa`'s [`sampleproject`](https://github.com/pypa/sampleproject) is a good reference for packaging and distributing.

Although `setuptools` has "evolved to only provide backend support, no longer being the de-facto packaging tool in the market" [2], it is still needed in many situations. See the section [**Development mode**](https://setuptools.readthedocs.io/en/latest/userguide/quickstart.html#development-mode) in the quickstart guide to see how to implement the `setup.py` in order to support "development mode" when `setup.cfg` is used.

### 1.1 Python Packaging Overview

First of all, read [An Overview of Packaging for Python](https://packaging.python.org/overview/). I don't understand why such an important topic is listed in the 4th place in the "Get started" section in the main page. That's why I think the list on the left panel makes more sense.

The overview document covers the major topics you want to know before everything else:

- Deployment for other developers (technical audience).
  - The section [Packaging Python libraries and tools](https://packaging.python.org/overview/#packaging-python-libraries-and-tools) covers this.
  - It also briefly explains two forms of distribution: **source distribution (sdist)** and **binary distribution (a.k.a. `wheel`)**.
  - It is a three-layer solution:
<img alt="Packaging for Python tools and libraries" src="https://packaging.python.org/_images/py_pkg_tools_and_libs.png" width=50% height=50% />
- Deployment for end-users (non-technical audience).
  - The section [Packaging Python applications](https://packaging.python.org/overview/#packaging-python-applications) covers this.
  - It is a seven-layer solution:
<img alt="Packaging for Python applications" src="https://packaging.python.org/_images/py_pkg_applications.png" width=50% height=50% />

### 1.2 Tutorials

The [Tutorials](https://packaging.python.org/tutorials/) page lists four step-by-step tutorials:

- Installing Packages
- Managing Application Dependencies
- [Packaging Python Projects](https://packaging.python.org/tutorials/packaging-projects/): This tutorial walks you through the most recent best practices of packaging a Python package. Do not rely on copying the packaging materials from an existing project because they may not use the current best practices (unless you need to consider backward compatibility).
- Creating Documentation

### 1.3 Guides

Excerpted from the page for the purpose of "Guides":

> Guides are focused on accomplishing a specific task and assume that you are already familiar with the basics of Python packaging. If you’re looking for an introduction to packaging, see _Tutorials_.

This section provides many valuable and helpful guides for specific tasks, such as:

- Including files in source distributions with `MANIFEST.in`
- Single-sourcing the package version
- Packaging namespace packages
- etc.

## 2. Wheels

See [`Wheels.md`](./Wheels.md).

## 3. tox

`tox` aims to "automate and standardize testing in Python. It is part of a larger vision of easing the packaging, testing and release process of Python software". [4]

## 4. Python Package Index

There are several options to host your own Python package server:

- [5] mentions two options:
  - [`devpi`](https://devpi.net/docs/devpi/devpi/latest/%2Bd/index.html) (which looks intimidating on the first glance but it's actually easy to use)
  - "Manual" repository
- [`Nexus` for PyPI repository](https://help.sonatype.com/repomanager3/formats/pypi-repositories).
- [`pypiserver`](https://pypi.org/project/pypiserver/) looks really neat!

## 5. `requirements.txt`

The process to determine `requirements.txt` is as follows:

1. Examine every `import` statement in the app.
2. Figure out what Python distribution(s), if any, are required to make that `import` statement work. (Some `import` statements refer to built-in modules of Python and so we don't need to add a distribution to requirements.txt to make them work.)
3. To make the `control` file, go find the "egg info" in `/usr/lib/python3/dist-packages` that corresponds to each distribution. Do `dpkg-query -S /path/to/egg/info`. This tells you the Debian package name that matches that distribution.
4. Test the build in the official build system to find special cases like "`gridfs` is not installed by default with `pymongo` .deb package" which we have no way of knowing a priori.

Note that if a package is only required for the build (e.g., only required for `setup.py`) and not actually to install/run the program, then it should be listed in the control file but not in `requirements.txt`.

## 6. References

- [1] [Python Packaging User Guide](https://packaging.python.org/)
- [2] [`setuptools` Quickstart](https://setuptools.readthedocs.io/en/latest/userguide/quickstart.html)
- [3] [Python Packaging Glossary](https://packaging.python.org/glossary/)
- [4] [tox](https://tox.readthedocs.io/en/latest/)
- [5] [Hosting your own simple repository](https://packaging.python.org/guides/hosting-your-own-index/)

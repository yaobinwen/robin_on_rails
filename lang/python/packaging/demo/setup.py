from setuptools import setup, find_packages

_LONG_DESC = """# Description

## Overview

This is the long description of the Python packaging demo.
"""

# For all the keyword arguments of `setup()`.
# See: https://setuptools.readthedocs.io/en/latest/setuptools.html#specifying-your-project-s-version
setup(
    name="my_pkg",  # Package name.

    # NOTE(ywen) >>>
    # `setuptools` has its own versioning scheme, but we should use semantic
    # versioning or the Debian versioning.
    # `pkg_resources.parse_version()` can be used to to compare different
    # version numbers.
    # See: https://setuptools.readthedocs.io/en/latest/setuptools.html#specifying-your-project-s-version
    # <<<
    version="0.1",  # Package version.

    description="Python packaging demo",
    long_description=_LONG_DESC,
    long_description_content_type="text/markdown",

    author="ywen",
    author_email="ywen@example.com",

    maintainer="ywen",
    maintainer_email="ywen@example.com",

    url="https://my_pkg.com", # Package homepage
    download_url="https://download.my_pkg.com", # URL to download the package

    # The packages that setuptools will manipulate.
    # Usually, this list is maintained manually, but for large packages, calling
    # `find_packages()` (or `find_namespace_packages()`) is the easiest way.
    #
    # NOTE(ywen) >>>
    # `find_packages()` is defined by renaming `PackageFinder.find`:
    # `find_packages = PackageFinder.find`. There is another `find_packages()`
    # that is defined by `def`.
    #
    # `find_namespace_packages()` is the renamed version of
    # `PEP420PackageFinder.find`.
    #
    # `PackageFinder.find()` is a `classmethod` that takes three arguments:
    # - 1). where='.'
    # - 2). exclude=()
    # - 3). include=('*',)
    #
    # The packages are searched under the directory `where`. Per the source
    # code comment: "It should be supplied as a "cross-platform" (i.e.
    # URL-style) path; it will be converted to the appropriate local path
    # syntax"
    #
    # Therefore, it looks like all the packages here will be put into the
    # distribution package.
    # <<<
    packages=find_namespace_packages(),

    # The modules that setuptools will manipulate.
    # TODO(ywen): What kind of manipulation?
    py_modules=["module_1", "module_2"],

    # The standalone script files to be built and installed.
    # TODO(ywen): Are they the scripts to be installed to `/usr/bin` (or alike)?
    scripts=["demo.py"],

    # A string specifying the base package name for the extensions provided by
    # this package.
    ext_package="",

    # A list of instances of setuptools.Extension providing the list of Python
    # extensions to be built.
    ext_modules=[],

    # A list of strings describing the categories for the package.
    classifiers=[
        "License :: OSI Approved :: Python Software Foundation License"
    ]

    # A subclass of Distribution to use.
    distclass=None,

    # A string specifying the name of the setup.py script – defaults to
    # sys.argv[0]
    # TODO(ywen): ???
    script_name="",

    # A list of strings defining the arguments to supply to the setup script.
    # TODO(ywen): ???
    script_args=[],

    # A dictionary providing the default options for the setup script.
    # TODO(ywen): ???
    options=dict(),

    # A string specifying the license of the package.
    license="",

    # A list of strings or a comma-separated string providing descriptive
    # meta-data. See: PEP 0314.
    # FIXME(ywen): Modify.
    keywords="python packaging demo",

    # A list of strings or comma-separated string.
    # TODO(ywen): ???
    platforms="",

    # A dictionary providing a mapping of command names to Command subclasses.
    cmdclass=dict(),

    # A list of strings specifying the data files to install.
    # **DEPRECATED**; should be avoided.
    data_files=list(),

    # A dictionary providing a mapping of package to directory names.
    package_dir=dict(),

    # **WARNING**: `requires` is superseded by `install_requires` and should
    # not be used anymore.
    requires=None,

    # List of strings describing packages which this package renders obsolete,
    # meaning that the two projects should not be installed at the same time.
    # **WARNING**: `obsoletes` is currently ignored by pip.
    obsoletes=list(),

    # List of strings describing package- and virtual package names contained
    # within this package.
    # **WARNING**: `provides` is currently ignored by pip.
    provides=list(),

    # If set to True, this tells `setuptools` to automatically include any data
    # files it finds inside your package directories that are specified by your
    # `MANIFEST.in` file.
    # TODO(ywen): ??? For more information, see the section on Including Data
    # Files.
    include_package_data=False,

    # A dictionary mapping package names to lists of glob patterns that should
    # be excluded from your package directories.
    exclude_package_data=dict(),

    # A dictionary mapping package names to lists of glob patterns.
    # TODO(ywen): ??? See "Including Data Files".
    package_data=dict(),

    # Whether the project can be safely installed and run from a zip file.
    zip_safe=False,

    # A string or list of strings specifying what other distributions need to
    # be installed when this one is.
    # See: https://setuptools.readthedocs.io/en/latest/setuptools.html#declaring-dependencies.
    install_requires=[
        "fun == 1.0.0",
        "black >= 20.8b1",
    ],

    # A dictionary mapping entry point group names to strings or lists of
    # strings defining the entry points. Entry points are used to support
    # dynamic discovery of services or plugins provided by a project.
    # TODO(ywen): ??? See Dynamic Discovery of Services and Plugins for details
    # and examples of the format of this argument. In addition, this keyword is
    # used to support Automatic Script Creation.
    entry_points=dict(),

    # A dictionary mapping names of "extras" (optional features of your project)
    # to strings or lists of strings specifying what other distributions must
    # be installed to support those features.
    # TODO(ywen): ??? See the section on Declaring Dependencies for details and
    # examples of the format of this argument.
    extras_require=dict(),

    # A string corresponding to a version specifier (as defined in PEP 440) for
    # the Python version, used to specify the Requires-Python defined in PEP
    # 345.
    # TODO(ywen): ???
    python_requires="",

    # A string or list of strings specifying what other distributions need to
    # be present in order for the setup script to run. setuptools will attempt
    # to obtain these (even going so far as to download them using EasyInstall)
    # before processing the rest of the setup script or commands. This argument
    # is needed if you are using distutils extensions as part of your build
    # process; for example, extensions that process setup() arguments and turn
    # them into EGG-INFO metadata files.
    # **WARNING**: Using setup_requires is discouraged in favor of PEP-518.
    setup_requires="",

    # A list of strings naming URLs to be searched when satisfying dependencies.
    # **DEPRECATED**: Not supported anymore by pip.
    dependency_links=list(),


    # A list of strings naming the project's "namespace packages".
    namespace_packages=list(),

    # A string naming a unittest.TestCase subclass (or a package or module
    # containing one or more of them, or a method of such a subclass), or naming
    # a function that can be called with no arguments and returns a
    # unittest.TestSuite. If the named suite is a module, and the module has an
    # additional_tests() function, it is called and the results are added to
    # the tests to be run. If the named suite is a package, any submodules and
    # subpackages are recursively added to the overall test suite.
    # New in 41.5.0: Deprecated the test command.
    # TODO(ywen): ???
    test_suite="",

    tests_require=list(),

    test_loader="",

    # A list of strings naming resources that should be extracted together.
    # TODO(ywen): ???
    eager_resources=list(),

    # Convert the source code from Python 2 to Python 3 with 2to3 during the
    # build process.
    use_2to3=False,

    # List of doctest source files that need to be converted with 2to3.
    # TODO(ywen): What is `doctest`?
    convert_2to3_doctests=list(),

    # A list of modules to search for additional fixers to be used during the
    # 2to3 conversion.
    use_2to3_fixers=list(),

    # List of fixer names to be skipped.
    use_2to3_exclude_fixers=list(),

    # An arbitrary map of URL names to hyperlinks, allowing more extensible
    # documentation of where various resources can be found than the simple
    # url and download_url options provide.
    project_urls={
        "Bug Tracker": "https://bugs.example.com/my_pkg/",
        "Documentation": "https://docs.example.com/my_pkg/",
        "Source Code": "https://code.example.com/my_pkg/",
    },
)

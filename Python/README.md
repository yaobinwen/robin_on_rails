# Python

## `__repr__()` vs `__str__()`

Refer to [Real Python: When Should You Use `.__repr__()` vs `.__str__()` in Python?](https://realpython.com/python-repr-vs-str/).

In short:

> The `.__repr__()` method returns a detailed description for a **programmer** who needs to maintain and debug the code. The `.__str__()` method returns a simpler description with information for the **user** of the program.

## Implicit Namespace Packages

This concept was introduced by [PEP 420](https://www.python.org/dev/peps/pep-0420/), but I think the PEP does a poor job to explain what an "implicit namespace package" is and what problem it tries to resolve. Instead, I found [this Stack Overflow answer](https://stackoverflow.com/a/21819733/630364) explains it really well. Also take a look at the official sample code: [Python Namespace Package Examples](https://github.com/pypa/sample-namespace-packages)

## Packaging

See [`python/packaging/README.md`](./python/packaging/README.md).

## Testing & Coverage

### Run Tests

Use native `unittest` module:
- `cd <package-root-dir>`
  - Typically, the root directory has `src` and `tests` sub-directories.
- `PYTHONPATH="./src" python3 -m unittest discover -v`
  - This command iterates the `tests` sub-directory recursively to find and run all the unit tests.

Use `pytest`:
- `cd <package-root-dir>`
  - Typically, the root directory has `src` and `tests` sub-directories.
- `PYTHONPATH="./src" python3 -m pytest -s .`
  - This command iterates the `tests` sub-directory recursively to find and run all the unit tests.

### Get Coverage (`Coverage.py`)

One can use [`Coverage.py`](https://coverage.readthedocs.io) to calculate the test coverage. It can be installed in two ways:
- `pip install coverage`
- `sudo apt install python3-coverage`

Run:
- `python3 -m coverage run -m unittest discover` (or `python -m coverage run`)
  - `coverage` can be run directly but using `python -m` or `python3 -m` can control which Python version to use.
- `coverage report -m` to report the missing lines.
- `coverage html` to generate HTML report.

### Get Coverage (`pytest-cov`)

Install: `sudo apt install python-pytest-cov` (or `python3-pytest-cov`).

Run:
- `cd <package-root-dir>`
  - Typically, the root directory has `src` and `tests` sub-directories.
- `python -m pytest -s --cov=. --cov-report=term-missing .`

## References

- [Real Python](https://realpython.com/)
- [Python 3 Module of the Week](https://pymotw.com/3/)
- [Idioms and Anti-Idioms in Python](https://docs.python.org/3.1/howto/doanddont.html)
- [Porting Python 2 Code to Python 3](https://docs.python.org/3/howto/pyporting.html)
- [Cheat Sheet: Writing Python 2-3 compatible code](http://python-future.org/compatible_idioms.html)
- [Python Developer's Guide](https://devguide.python.org/) has a lot resources about the development of Python itself.

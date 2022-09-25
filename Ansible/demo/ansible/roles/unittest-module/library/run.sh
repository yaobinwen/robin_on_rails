#!/bin/sh

set -ex

# Run the unit tests using Python's `unittest` instead of `pytest`.
python -m unittest discover -v

# NOTE(ywen): However, one can also test the code using `pytest`.
# python -m pytest -v --cov . --cov-report=term-missing .

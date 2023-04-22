# Network Demo

## Overview

This demo implements the network mechanisms that are described in the book [1].

To run the demo: `PYTHONPATH="./src" ./demo_<name>.py`

To run the unit tests: `PYTHONPATH="./src" python3 -m unittest discover -v`

To run the unit tests with `coverage`:
- Install `python3-coverage`: `sudo apt install python3-coverage`.
- Run the tests: `PYTHONPATH="./src" python3-coverage run -m unittest discover -v`
- View the report: `python3-coverage report -m`

## TODOs

- Chapter 1:
  - Page 16, `Getting to Know You`: Implement the broadcast to find the target MAC address.
  - Page 19, `The Two Aspects of NICs`: Implement the two sub-layers.

## References

- [1] [CompTIA Network+ Certification All-in-One Exam Guide, Seventh Edition](https://www.amazon.com/CompTIA-Network-Certification-Seventh-N10-007/dp/1260122387)

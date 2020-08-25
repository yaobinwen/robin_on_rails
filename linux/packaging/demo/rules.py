#!/usr/bin/env python3

import argparse
import re
import sys


def _enable_verbose(lines):
    # Firstly, try to determine if the verbose mode is already on.
    enabled = False
    p = re.compile("^export[ ]+DH_VERBOSE[ ]*=[ ]*1[ ]*$")
    for line in lines:
        if p.match(line) is not None:
            enabled = True
            break

    if not enabled:
        # Try to find the line for DH_VERBOSE and enable that line.
        p = re.compile("export[ ]+DH_VERBOSE")
        for i in range(0, len(lines)):
            if p.search(lines[i]) is not None:
                lines[i] = "export DH_VERBOSE = 1\n"
                enabled = True
                break

        if not enabled:
            lines.insert(1, "export DH_VERBOSE = 1\n")

    return lines


def _read(fpath):
    lines = []
    with open(fpath, "r") as fh:
        line = fh.readline()
        while line:
            # If line is empty, EOF is reached. An empty line is returned as
            # a single '\n'. See:
            # https://docs.python.org/3/tutorial/inputoutput.html#methods-of-file-objects
            lines.append(line)
            line = fh.readline()

    return lines


def _syntax():
    p = argparse.ArgumentParser(description="Manipulate the debian rules file.")

    p.add_argument("rules_path", help="The path of 'rules' file to manipulate.")
    p.add_argument(
        "--enable-verbose",
        action="store_true",
        default=False,
        help="Enable verbose mode.",
    )

    return p


def _write(fpath, lines):
    with open(fpath, "w") as fh:
        fh.writelines(lines)


def main(rules_path, enable_verbose):
    if enable_verbose:
        lines = _read(rules_path)
        lines = _enable_verbose(lines)
        _write(rules_path, lines)


if __name__ == "__main__":
    sys.exit(main(**vars(_syntax().parse_args())))

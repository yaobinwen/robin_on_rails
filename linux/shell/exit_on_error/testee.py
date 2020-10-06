#!/usr/bin/python3

import argparse
import sys


def _syntax():
    parser = argparse.ArgumentParser(
        description=(
            "A test program that either succeeds or fails on demand with a "
            "specified message."
        ),
    )

    parser.add_argument(
        "result",
        choices=["success", "failure"],
        help="Whether the program should exit with success or failure.",
    )
    parser.add_argument(
        "--message", "-m",
        help="The message to print.",
    )

    return parser


def main(message, result):
    if message is not None:
        print(message)

    return 0 if result == "success" else 1


if __name__ == "__main__":
    sys.exit(main(**vars(_syntax().parse_args())))

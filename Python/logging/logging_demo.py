#!/usr/bin/env python

'''This file contains examples of using `logging`. For its documentation, see
https://docs.python.org/2/library/logging.html.

The documentation provides a basic tutorial:
https://docs.python.org/2/howto/logging.html#logging-basic-tutorial
'''


import logging
import sys
import unittest


def main():
    try:
        raise Exception("Wrong!")
    except Exception as ex:
        logging.error('Error occurred: %s', str(ex))


class LoggingBehaviors(unittest.TestCase):
    pass


if __name__ == '__main__':
    unittest.main()

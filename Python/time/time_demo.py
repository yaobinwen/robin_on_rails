#!/usr/bin/env python3


"""This file contains examples of using `time`. For its documentation, see
https://docs.python.org/3/library/time.html
This document is referred to as `DOC` in the code.
"""

import time
import unittest


class TestTime(unittest.TestCase):

    def test_time(self):
        """Demo for `time.time`: Return the time in seconds since the epoch as
        a floating point number.

        https://docs.python.org/3/library/time.html#time.time
        """
        secs_since_epoch = time.time()
        self.assertIsInstance(secs_since_epoch, float)


if __name__ == '__main__':
    unittest.main()

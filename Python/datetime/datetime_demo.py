#!/usr/bin/env python3


"""This file contains examples of using `datetime`. For its documentation, see
https://docs.python.org/3/library/datetime.html
This document is referred to as `DOC` in the code.
"""

import unittest

from datetime import timedelta


class Test_timedelta(unittest.TestCase):
    def test_init(self):
        """Demo of `datetime.timedelta.__init__`.

        https://docs.python.org/3/library/datetime.html#datetime.timedelta
        """

        # By default, construct a time delta of 0.
        # `timedelta` only stores `days`, `seconds`, and `microseconds`
        # internally, so there are only these three members.
        td = timedelta()
        self.assertEqual(td.days, 0)
        self.assertEqual(td.seconds, 0)
        self.assertEqual(td.microseconds, 0)

        # Arguments other than `days`, `seconds`, and `microseconds` are
        # merged into them appropriately.
        td = timedelta(seconds=10, milliseconds=2010, microseconds=20)
        self.assertEqual(td.days, 0)
        # `milliseconds` is not stored. In this case, it is converted to
        # as many seconds as possible by dividing it by 1000.
        ms_to_s = 2010 // 1000
        self.assertEqual(td.seconds, 10 + ms_to_s)
        # The remaining part of `milliseconds` is converted to `microseconds`
        # by multiplying 1000.
        ms_to_us = (2010 % 1000) * 1000
        self.assertEqual(td.microseconds, 20 + ms_to_us)


if __name__ == '__main__':
    unittest.main()

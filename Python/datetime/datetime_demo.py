#!/usr/bin/env python3


"""This file contains examples of using `datetime`. For its documentation, see
https://docs.python.org/3/library/datetime.html
This document is referred to as `DOC` in the code.
"""

import unittest

from datetime import datetime, timedelta, timezone


class Test_datetime(unittest.TestCase):
    def test_today(self):
        """Demo of `datetime.datetime.today()`.

        https://docs.python.org/3/library/datetime.html#datetime.datetime.today

        This method returns the local time but WITHOUT time zone info.
        Therefore, simply looking at the returned object by `today()` one would
        not be able to tell if it's UTC or local time.

        Therefore, if we assume that one computer is always in one time zone,
        `today()` is the best to be used on the same local computer without
        passing it anywhere else, not even on another local computer because
        the other computer may be in a different time zone.
        """
        today = datetime.today()
        print(f"today: {today} (local time, NO time zone info)")
        self.assertTrue(today.year > 0)
        self.assertTrue(today.month > 0)
        self.assertTrue(today.day > 0)
        self.assertTrue(today.hour >= 0)
        self.assertTrue(today.minute >= 0)
        self.assertTrue(today.second >= 0)
        self.assertTrue(today.microsecond >= 0)

        # The `today` object doesn't have any time zone info, making the user
        # unable to tell if it is a UTC time or a local time.
        self.assertIsNone(today.tzinfo)

    def test_now(self):
        """Demo of `datetime.datetime.now()`.

        https://docs.python.org/3/library/datetime.html#datetime.datetime.now
        """
        now_no_tz = datetime.now()

        tz_est = timezone(offset=timedelta(hours=-4), name="EST")
        now_with_tz = datetime.now(tz=tz_est)

        now_in_utc = datetime.now(tz=timezone.utc)

        print(f"now without tz: {now_no_tz}")
        print(f"   now with tz: {now_with_tz}")
        print(f"    now in UTC: {now_in_utc}")

    def test_utcnow(self):
        """Demo of `datetime.datetime.utcnow()`.

        https://docs.python.org/3/library/datetime.html#datetime.datetime.utcnow

        `utcnow()` has the same issue as `today()` that it doesn't contain the
        time zone info, so by looking at the `utcnow` object, one can't tell if
        it is a UTC time or some local time. So `now(tz=timezone.utc)` is
        preferred.
        """
        utcnow = datetime.utcnow()
        now_in_utc = datetime.now(tz=timezone.utc)

        print(f"    utcnow: {utcnow}")
        print(f"now_in_utc: {now_in_utc}")


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

#!/usr/bin/env python3


"""This file contains examples of using `datetime`. For its documentation, see
https://docs.python.org/3/library/datetime.html
This document is referred to as `DOC` in the code.
"""

import sys
import unittest

from datetime import date, datetime, timedelta, timezone


PY_3 = sys.version_info.major == 3
PY_MINOR_VERSION = sys.version_info.minor


class Test_date(unittest.TestCase):
    def test_date(self):
        """Demo of `datetime.date`.

        https://docs.python.org/3/library/datetime.html#datetime.date
        """
        d = date(year=2023, month=11, day=18)
        self.assertEqual(d.year, 2023)
        self.assertEqual(d.month, 11)
        self.assertEqual(d.day, 18)

    @unittest.skipIf(PY_3 and PY_MINOR_VERSION < 7, reason="Python version is too low")
    def test_fromisoformat(self):
        """Demo of `datetime.date.fromisoformat()`.

        https://docs.python.org/3/library/datetime.html#datetime.date.fromisoformat
        """
        d = date.fromisoformat("2023-11-18")
        self.assertEqual(d.year, 2023)
        self.assertEqual(d.month, 11)
        self.assertEqual(d.day, 18)


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

    def test_strptime(self):
        """Demo of `datetime.datetime.strptime()`

        https://docs.python.org/3/library/datetime.html#datetime.datetime.strptime
        """
        dt = datetime.strptime(
            "2024-07-13T02:38:38.388392-0400", "%Y-%m-%dT%H:%M:%S.%f%z"
        )
        self.assertEqual(dt.year, 2024)
        self.assertEqual(dt.month, 7)
        self.assertEqual(dt.day, 13)
        self.assertEqual(dt.hour, 2)
        self.assertEqual(dt.minute, 38)
        self.assertEqual(dt.second, 38)
        self.assertEqual(dt.microsecond, 388392)
        self.assertEqual(dt.tzinfo, timezone(offset=timedelta(hours=-4), name="EST"))

    def test_timestamp(self):
        """Demo of `datetime.datetime.timestamp()`

        https://docs.python.org/3/library/datetime.html#datetime.datetime.timestamp
        """
        dt = datetime(
            year=2024,
            month=1,
            day=31,
            hour=12,
            minute=25,
            second=47,
            microsecond=123,
            tzinfo=timezone.utc,
        )
        # Timestamp is a float.
        self.assertIsInstance(dt.timestamp(), float)
        self.assertAlmostEqual(dt.timestamp(), 1706703947.000123)


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

    def test_calculated_from_datetime(self):
        """Demo of calculating time delta from two `datetime` objects.
        """
        dt1 = datetime(
            year=2024,
            month=4,
            day=1,
            hour=16,
            minute=0,
            second=0,
            microsecond=0,
            tzinfo=timezone.utc,  # Use UTC for less ambiguity.
        )
        dt2 = datetime(
            year=2024,
            month=4,
            day=1,
            hour=16,
            minute=0,
            second=14,
            microsecond=393,
            tzinfo=timezone.utc,  # Use UTC for less ambiguity.
        )
        td = dt2 - dt1
        self.assertEqual(td.days, 0)
        self.assertEqual(td.seconds, 14)
        self.assertEqual(td.microseconds, 393)


if __name__ == "__main__":
    unittest.main()

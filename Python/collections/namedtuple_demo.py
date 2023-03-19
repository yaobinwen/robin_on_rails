#!/usr/bin/env python3

"""
This file contains example of using collections.namedtuple. For its
documentation, see https://docs.python.org/3/library/collections.html
This document is referred to as `DOC` in the code.
"""

import json
import unittest

from collections import namedtuple


class TestNamedtuple(unittest.TestCase):

    def test_json_dump(self):
        MyEvent = namedtuple(
            typename="MyEvent",
            field_names=["type", "timestamp"],
        )

        e = MyEvent(type="changed", timestamp="t1")
        s_namedtuple = json.dumps(e)
        s_dict = json.dumps({
            "type": e.type,
            "timestamp": e.timestamp,
        })

        # JSON dump of a namedtuple is not like a dictionary.
        self.assertNotEqual(s_namedtuple, s_dict)


if __name__ == '__main__':
    unittest.main()

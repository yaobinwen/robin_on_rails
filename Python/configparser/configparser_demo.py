#!/usr/bin/python3

"""
This file contains example of using configparser. For its documentation,
see https://docs.python.org/3/library/configparser.html
This document is referred to as `DOC` in the code.
"""

import configparser

TEST_CONFIG = """
[Address]
Address=addr1

[Address]
Address=addr2

[Network]
Address=addr3
Address=addr4
"""

c = configparser.ConfigParser(strict=False)
c.read_string(TEST_CONFIG)
print(c["Address"]["Address"]) # prints "addr2".
print(c["Network"]["Address"]) # prints "addr4".

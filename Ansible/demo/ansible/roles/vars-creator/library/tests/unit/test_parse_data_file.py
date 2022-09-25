from __future__ import (
    absolute_import, division, print_function, unicode_literals
)

import unittest


class TestImport(unittest.TestCase):
    def test_import(self):
        # Make sure the module can be imported.
        import parse_data_file


if __name__ == "__main__":
    unittest.main()

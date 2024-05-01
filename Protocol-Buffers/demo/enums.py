import unittest

from google.protobuf import json_format
from protobufs.v0.address_pb2 import STATE_NY, State


class Test_enum(unittest.TestCase):
    """Demo how to use Protocal Buffers' `enum`.
    """

    def test_enum_type(self):
        """Demo that enums are integers.
        """
        self.assertIsInstance(STATE_NY, int)
        self.assertEqual(STATE_NY, 32)

    def test_enum_access(self):
        """Demo how to access the enums in a compiled `_pb2` module.
        """
        # `State` is an `EnumTypeWrapper` which doesn't have the enum `STATE_NY`,
        # so one can't access it as `State.STATE_NY`.
        self.assertFalse(hasattr(State, "STATE_NY"))

        self.assertRaises(AttributeError, lambda: State.STATE_NY)


if __name__ == "__main__":
    unittest.main()

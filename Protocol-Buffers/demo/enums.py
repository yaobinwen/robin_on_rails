import unittest

from google.protobuf import json_format
from protobufs.v0.address_pb2 import STATE_UNSPECIFIED, STATE_NY, State


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

        # The concrete enum values such as `STATE_NY` is not owned by the enum
        # itself `State`, so accessing `State.STATE_NY` will raise an error.
        self.assertRaises(AttributeError, lambda: State.STATE_NY)

    def test_enum_enumerate_items(self):
        """Demo how to enumerate all the possible values of an enum.
        """
        # Call `items()` to enumerate the enum's values and corresponding names.
        # Example output:
        # [('STATE_UNSPECIFIED', 0), ('STATE_AL', 1), ('STATE_AK', 2), ...]
        items = State.items()
        self.assertIsInstance(items, list)

        for item in items:
            # Each item is a 2-element tuple.
            self.assertIsInstance(item, tuple)
            self.assertEqual(len(item), 2)
            # The first element is a string that represents the name of the
            # enum value.
            self.assertIsInstance(item[0], str)
            # The second element is an integer that represents the numeric value
            # of the enum value.
            self.assertIsInstance(item[1], int)

    def test_enum_enumerate_keys(self):
        """Demo how to enumerate all the keys of an enum.
        """
        # The keys are the names of the enum values, such as the string
        # "STATE_AL", "STATE_NY", etc.
        keys = State.keys()
        self.assertIsInstance(keys, list)

        for key in keys:
            # Each key is a string that represents the name of the enum value.
            self.assertIsInstance(key, str)

    def test_enum_enumerate_values(self):
        """Demo how to enumerate all the values of an enum.
        """
        # The values are the numeric values of the enum values, such as 1 for
        # STATE_AL, 32 for STATE_NY, etc.
        values = State.values()
        self.assertIsInstance(values, list)

        for value in values:
            # Each value is a numeric value for that enum value.
            self.assertIsInstance(value, int)

    def test_get_str_representation_of_enum(self):
        """Demo how to get the string representation of an enum value given its
        numeric value.
        """
        # Call the `Name` method to get the string representation.
        self.assertEqual(State.Name(0), "STATE_UNSPECIFIED")
        self.assertEqual(State.Name(STATE_NY), "STATE_NY")

        # Out-of-range values will cause `ValueError`:
        self.assertRaises(ValueError, State.Name, -9999)

    def test_get_numeric_representation_of_enum(self):
        """Demo how to get the numeric value of an enum value given given its
        string representation.
        """
        # Call the `Value` method to get the numeric values.
        self.assertEqual(State.Value("STATE_UNSPECIFIED"), STATE_UNSPECIFIED)
        self.assertEqual(State.Value("STATE_NY"), STATE_NY)

        # Non-recognizable strings will cause `ValueError`.
        self.assertRaises(ValueError, State.Value, "STATE_IMAGINATION")


if __name__ == "__main__":
    unittest.main()

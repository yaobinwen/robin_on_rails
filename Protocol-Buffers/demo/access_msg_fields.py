import unittest

from protobufs.v0.building_pb2 import Building
from protobufs.v0.maps_pb2 import Management


class Test_access_simple_fields(unittest.TestCase):
    """Demo how to access a message's simple fields (i.e., non-nested fields).
    """
    def setUp(self):
        self.b1 = Building(name="Grand 1", address="191-01 41 Ave.", years_since_built=10)

    def test_access_string(self):
        # Read a string field.
        self.assertEqual(self.b1.name, "Grand 1")

        # Update a string field.
        self.b1.name = "Grand A"
        self.assertEqual(self.b1.name, "Grand A")

    def test_access_uint32(self):
        # Read a uint32 field.
        self.assertEqual(self.b1.years_since_built, 10)

        # Update a uint32 field.
        self.b1.years_since_built = 11
        self.assertEqual(self.b1.years_since_built, 11)


class Test_access_map_fields(unittest.TestCase):
    """Demo how to access a message's `map` fields.
    """
    def setUp(self):
        self.b1 = Building(name="Grand 1", address="191-01 41 Ave.", years_since_built=10)
        self.b2 = Building(name="Grand 2", address="191-02 42 Ave.", years_since_built=5)
        self.managers = {"Ada": self.b1, "Bob": self.b2}
        self.management = Management(managers=self.managers)

    def test_read_map_values_from_keys(self):
        print(dir(self.management.managers))

        m = self.management.managers

        # `keys()` returns a `KeyView` that can be converted into a set.
        manager_names = set(m.keys())
        self.assertSetEqual(set(self.managers.keys()), manager_names)

        # Iterate through the keys.
        for manager_name in m.keys():
            self.assertIn(manager_name, set(self.managers.keys()))

        # Get the value of the keys.
        b1 = m.get("Ada")
        ## `b1` and `self.b1`'s contents are the same (i.e., their field values
        ## are the same).
        self.assertEqual(b1, self.b1)
        self.assertEqual(b1.name, self.b1.name)
        self.assertEqual(b1.address, self.b1.address)
        self.assertEqual(b1.years_since_built, self.b1.years_since_built)
        ## But `b1` and `self.b1` don't point to the same object.
        self.assertNotEqual(id(b1), id(self.b1))
        ## Updating `b1` doesn't affect `self.b1`.
        b1.name = "Grand A"
        self.assertEqual(b1.name, "Grand A")
        self.assertEqual(self.b1.name, "Grand 1")


if __name__ == "__main__":
    unittest.main()

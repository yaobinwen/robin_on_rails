import unittest

from google.protobuf import json_format
from protobufs.v0.building_pb2 import Building
from protobufs.v0.maps_pb2 import Management


class Test_map(unittest.TestCase):
    """Demo how to use Protocal Buffers' `map`.
    """

    def setUp(self):
        self.b1 = Building(
            name="Grand 1", address="191-01 41 Ave.", years_since_built=1
        )
        self.b2 = Building(
            name="Grand 2", address="191-02 42 Ave.", years_since_built=2
        )
        self.managers = {"Ada": self.b1, "Bob": self.b2}
        self.management = Management(managers=self.managers)

    def test_initialize_map(self):
        """Demo how to initialize a `map` field using a Python `dict`.
        """
        d = json_format.MessageToDict(
            message=self.management,
            including_default_value_fields=True,
            preserving_proto_field_name=True,
        )

        self.assertEqual(len(d["managers"]), len(self.managers))

        # Ada's managed building.
        self.assertIn("Ada", d["managers"])
        b = d["managers"]["Ada"]
        self.assertEqual(b["name"], "Grand 1")
        self.assertEqual(b["address"], "191-01 41 Ave.")
        self.assertEqual(b["years_since_built"], 1)

        # Bob's managed building.
        self.assertIn("Bob", d["managers"])
        b = d["managers"]["Bob"]
        self.assertEqual(b["name"], "Grand 2")
        self.assertEqual(b["address"], "191-02 42 Ave.")
        self.assertEqual(b["years_since_built"], 2)

    def test_update_message(self):
        b2_ = Building(
            name="Grand B",
            address=self.b2.address,
            years_since_built=self.b2.years_since_built,
        )
        b3 = Building(name="Grand C", address="191-C 42 Ave.", years_since_built=30)
        managers2 = {"Bob": b2_, "Charlie": b3}
        management2 = Management(managers=managers2)

        self.management.MergeFrom(management2)

        m = self.management.managers
        b1 = m.get("Ada")
        self.assertEqual(b1, self.b1)
        b2 = m.get("Bob")
        self.assertNotEqual(b2.name, self.b2.name)
        self.assertEqual(b2.address, self.b2.address)
        self.assertEqual(b2.years_since_built, self.b2.years_since_built)
        b3 = m.get("Charlie")
        self.assertEqual(
            b3, Building(name="Grand C", address="191-C 42 Ave.", years_since_built=30)
        )


if __name__ == "__main__":
    unittest.main()

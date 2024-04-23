import copy
import unittest

from google.protobuf import json_format
from protobufs.v0.building_pb2 import Building
from protobufs.v0.maps_pb2 import Management


class Test_map(unittest.TestCase):
    """Demo how to use Protocal Buffers' `map`.
    """

    def test_initialize_map(self):
        """Demo how to initialize a `map` field using a Python `dict`.
        """
        managers = {
            "Ada": Building(
                name="Grand 1", address="191-01 41 Ave.", years_since_built=1
            ),
            "Bob": Building(
                name="Grand 2", address="191-02 42 Ave.", years_since_built=2
            ),
        }
        management = Management(managers=managers)
        d = json_format.MessageToDict(
            message=management,
            including_default_value_fields=True,
            preserving_proto_field_name=True,
        )

        self.assertEqual(len(d["managers"]), len(managers))

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
        b1 = Building(name="Grand 1", address="191-01 41 Ave.", years_since_built=1)
        b2 = Building(name="Grand 2", address="191-02 42 Ave.", years_since_built=2)
        managers = {"Ada": b1, "Bob": b2}
        management = Management(managers=managers)

        d1 = json_format.MessageToDict(
            message=management,
            including_default_value_fields=True,
            preserving_proto_field_name=True,
        )

        # Update Bob's last check time by making a copy of the existing message.

        d2 = copy.deepcopy(d1)


if __name__ == "__main__":
    unittest.main()

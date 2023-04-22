import unittest

from network.device import Device


class TestDevice(unittest.TestCase):
    def test_init(self):
        self.assertEqual(Device(name="d1").name, "d1")

        for n in (None, 10, [], {}):
            self.assertRaises(AssertionError, Device, name=n)


if __name__ == "__main__":
    unittest.main()

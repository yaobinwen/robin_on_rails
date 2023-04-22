import unittest

from network.mac_addr import MACAddr


class TestMACAddr(unittest.TestCase):
    def test_init(self):
        self.assertRaises(TypeError, MACAddr, ["0123", "4567", "89AB"])

        # Raise `ValueError`.
        for addr in (
            "0123456789ABC",   # Longer than 12 chars.
            "0123456789A",   # Shorter than 12 chars.
            "0123456789XY", # Exactly 12 chars but with non-hex chars.
        ):
            self.assertRaises(ValueError, MACAddr, addr)

    def test_eq(self):
        self.assertEqual(
            MACAddr("0123456789AB"),
            MACAddr("0123456789AB"),
        )

        self.assertEqual(
            MACAddr("0123456789AB"),
            MACAddr("0123456789ab"),
        )

    def test_ne(self):
        self.assertNotEqual(
            MACAddr("0123456789AB"),
            MACAddr("0123456789CD"),
        )

        self.assertNotEqual(
            MACAddr("0123456789AB"),
            MACAddr("0123456789cd"),
        )

    def test_repr(self):
        self.assertEqual(
            repr(MACAddr("0123456789AB")),
            "MACAddr<addr='0123456789AB',>",
        )

        self.assertEqual(
            repr(MACAddr("0123456789cd")),
            "MACAddr<addr='0123456789CD',>",
        )

    def test_hash(self):
        addr1 = MACAddr("0123456789AB")
        addr2 = MACAddr("0123456789cd")
        d = {
            addr1: "0123456789AB",
            addr2: "0123456789CD",
        }
        self.assertIn(addr1, d)
        self.assertIn(addr2, d)


if __name__ == "__main__":
    unittest.main()

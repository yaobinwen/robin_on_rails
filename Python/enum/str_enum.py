import enum
import unittest


NAME_ALICE = "Alice"
NAME_BOB = "Bob"


class Name(enum.StrEnum):
    ALICE = NAME_ALICE
    BOB = NAME_BOB

    @staticmethod
    def has_value(s):
        return s == Name.ALICE or s == Name.BOB


class TestEnum(unittest.TestCase):
    def test_has_value(self):
        self.assertTrue(Name.has_value(NAME_ALICE))
        self.assertTrue(Name.has_value(Name.ALICE))

        self.assertFalse(Name.has_value("alicE"))


if __name__ == "__main__":
    unittest.main()

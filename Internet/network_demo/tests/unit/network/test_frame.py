import unittest
import zlib

from network.frame import Frame
from network.mac_addr import MACAddr


class TestFrame(unittest.TestCase):
    def test_init(self):
        addr_src = MACAddr("11112222aaaa")
        addr_dest = MACAddr("11112222bbbb")

        self.assertRaises(
            AssertionError, Frame,
            eof=1, dest_mac_addr="", src_mac_addr=addr_src, data_type=0,
            data_bytes=b"",
        )

        self.assertRaises(
            AssertionError, Frame,
            eof=1, dest_mac_addr=addr_dest, src_mac_addr=None, data_type=0,
            data_bytes=b"",
        )

        for dt in (-1, 256):
            self.assertRaises(
                AssertionError, Frame,
                eof=1, dest_mac_addr=addr_dest, src_mac_addr=addr_src,
                data_type=dt, data_bytes=b"",
            )

        self.assertRaises(
            AssertionError, Frame,
            eof=1, dest_mac_addr=addr_dest, src_mac_addr=addr_src, data_type=0,
            data_bytes=bytearray(Frame.DATA_BYTES_MAX_LENGTH+1),
        )

        f = Frame(
            eof=1, dest_mac_addr=addr_dest, src_mac_addr=addr_src, data_type=0,
            data_bytes=bytearray(Frame.DATA_BYTES_MAX_LENGTH),
        )
        self.assertIsNotNone(f.fcs)

    def test_get_bytes(self):
        addr_src = MACAddr("11112222aaaa")
        addr_dest = MACAddr("11112222bbbb")
        f = Frame(
            eof=1, dest_mac_addr=addr_dest, src_mac_addr=addr_src,
            data_type=17, data_bytes=b"123",
        )
        crc = zlib.crc32(b"123") & 0xffffffff
        b = f.get_bytes()
        self.assertEqual(b[0], 1)
        self.assertEqual(b[1:13].decode("UTF-8"), "11112222bbbb")
        self.assertEqual(b[13:25].decode("UTF-8"), "11112222aaaa")
        self.assertEqual(b[25], 17)
        self.assertEqual(b[26:29], b"123")
        self.assertEqual(int.from_bytes(bytes=b[29:], byteorder="big"), crc)

    def test_from_bytes(self):
        addr_src = MACAddr("11112222aaaa")
        addr_dest = MACAddr("11112222bbbb")
        f = Frame(
            eof=1, dest_mac_addr=addr_dest, src_mac_addr=addr_src,
            data_type=17, data_bytes=b"123",
        )
        crc = zlib.crc32(b"123") & 0xffffffff
        b = f.get_bytes()

        f2 = Frame.from_bytes(b)

        self.assertEqual(f.eof, f2.eof)
        self.assertEqual(f.dest_mac_addr, f2.dest_mac_addr)
        self.assertEqual(f.src_mac_addr, f2.src_mac_addr)
        self.assertEqual(f.data_type, f2.data_type)
        self.assertEqual(f.data_bytes, f2.data_bytes)
        self.assertEqual(f.fcs, f2.fcs)


if __name__ == "__main__":
    unittest.main()

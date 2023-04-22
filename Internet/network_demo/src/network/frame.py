import zlib

from network.mac_addr import MACAddr
from network.osi_layers import OSILayer2
from network.pdu import ProtocolDataUnit


class Frame(OSILayer2, ProtocolDataUnit):
    """A Layer 1 frame.
    """

    MAX_LENGTH = 1500

    DATA_BYTES_MAX_LENGTH = (
        MAX_LENGTH
        - 1 # The length of `eof`.
        - MACAddr.LENGTH * 2    # The total length of the two MAC addresses.
        - 1 # The length of `data_type`.
        - int(32 / 8)  # The length of CRC32 checksum.
    )

    def __init__(
        self,
        eof: int,
        dest_mac_addr: MACAddr,
        src_mac_addr: MACAddr,
        data_type: int,
        data_bytes: bytes,
    ):
        # NOTE(ywen): It's OK to use asserts here because the input arguments
        # do not come from an external source. When the input arguments come
        # from an external source so they *could* be wrong, we need explicit
        # validation using `if`.
        assert isinstance(eof, int)
        assert isinstance(dest_mac_addr, MACAddr)
        assert isinstance(src_mac_addr, MACAddr)
        assert isinstance(data_type, int)
        assert 0 <= data_type and data_type <= 255
        assert len(data_bytes) <= Frame.DATA_BYTES_MAX_LENGTH, (
            f"Fail: {len(data_bytes)} <= {Frame.DATA_BYTES_MAX_LENGTH}"
        )

        self.eof = eof
        self.dest_mac_addr = dest_mac_addr
        self.src_mac_addr = src_mac_addr
        self.data_type = data_type
        self.data_bytes = data_bytes

        self.fcs = zlib.crc32(self.data_bytes) & 0xffffffff

    def get_bytes(self):
        b = bytearray()

        b.append(self.eof)
        b.extend(self.dest_mac_addr.addr.encode("UTF-8"))
        b.extend(self.src_mac_addr.addr.encode("UTF-8"))
        b.append(self.data_type)
        b.extend(self.data_bytes)
        # TODO(ywen): Confirm the endianness of network protocols.
        b.extend(self.fcs.to_bytes(length=int(32/8), byteorder="big"))

        assert len(b) <= Frame.MAX_LENGTH

        return b

    @classmethod
    def from_bytes(cls, frame_bytes: bytes):
        eof = frame_bytes[0]
        dest_addr = frame_bytes[1:13].decode("UTF-8")
        src_addr = frame_bytes[13:25].decode("UTF-8")
        data_type = frame_bytes[25]
        data_bytes = frame_bytes[26:-4]
        fcs = int.from_bytes(bytes=frame_bytes[-4:], byteorder="big")

        f = cls(
            eof=eof,
            dest_mac_addr=MACAddr(addr=dest_addr),
            src_mac_addr=MACAddr(addr=src_addr),
            data_type=data_type,
            data_bytes=data_bytes,
        )

        assert f.fcs == fcs

        return f

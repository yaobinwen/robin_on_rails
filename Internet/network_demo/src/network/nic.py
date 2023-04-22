import logging
import zlib

from network.device import Device
from network.frame import Frame
from network.osi_layers import OSILayer1, OSILayer2


logger = logging.getLogger("nic")


class NIC(OSILayer1, OSILayer2, Device):
    """Network Interface Card
    """

    def __init__(
        self,
        name: str,
        hub: "Hub"=None,
        mac_addr: "MACAddr"=None,
    ):
        """Initialize.

        Args:
            name: The name of this NIC.
            hub: The hub that this NIC connects to.
            mac_addr: The MAC address of this NIC.
        """
        super().__init__(name=name)

        self.hub = hub
        self.mac_addr = mac_addr
        self.data_bytes_segs = []

    def __repr__(self):
        return (
            "NIC<"
            f"mac={self.mac_addr},"
            f"name={self.name},"
            ">"
        )

    def connect(self, hub: "Hub"):
        """Connect this NIC with the given Hub.

        Args:
            hub: The hub to connect to.
        """
        self.hub = hub
        logger.info(f"'{self}' is connected to '{self.hub}'.")

    def disconnect_hub(self):
        """Disconnect this NIC with the hub.
        """
        hub = self.hub
        self.hub = None
        logger.info(f"'{self}' is disconnected from '{hub}'.")

    def recv(self, data_bytes: bytes):
        """Receive data from the source NIC.

        Args:
            data_bytes: The bytes of the data.
        """
        logger.info(f"'{self}' receives data: {len(data_bytes)} byte(s).")

        f = Frame.from_bytes(data_bytes)

        if f.dest_mac_addr != self.mac_addr:
            # The data is not for me. Just drop it.
            logger.info(f"'{self}': Received data is not for me. Drop it.")
            return

        # Check the FCS.
        fcs_expected = zlib.crc32(f.data_bytes) & 0xffffffff
        if fcs_expected != f.fcs:
            # FCS fails. The data is corrupted so just drop it.
            logger.warn(
                "Received data is corrupted: "
                f"expected FCS '{fcs_expected}'; actual FCS: '{f.fcs}'."
            )
            return

        # Always append the data bytes to the segments because when EOF is True
        # we will recreate the entire data bytes using the buffer.
        self.data_bytes_segs.append(f.data_bytes)

        if f.eof:
            # EOF. Assemble the bytes segments together.
            b = bytearray()
            for s in self.data_bytes_segs:
                b.extend(s)

            data = b.decode("UTF-8")
            logger.info(f"'{self}' receives data (in UTF-8): '{data}'")

            self.data_bytes_segs.clear()

    def send(self, data: str, encoding: str, dest_nic: "NIC"):
        """Send given data to the destination NIC.

        Args:
            data: The data to be sent.
            encoding: The encoding to transform the data.
            dest_nic: The destination NIC of the data.
        """
        if self.hub is None:
            raise RuntimeError(f"'{self}' is not connected to any hub.")

        b = data.encode(encoding=encoding)

        # If the data is too long to be sent in one frame, we send them in
        # multiple frames.
        t = len(b) // Frame.DATA_BYTES_MAX_LENGTH + 1
        start = 0
        end = Frame.DATA_BYTES_MAX_LENGTH
        for i in range(0, t):
            start = i * Frame.DATA_BYTES_MAX_LENGTH
            end = (i+1) * Frame.DATA_BYTES_MAX_LENGTH
            f = Frame(
                eof=(i == t - 1),
                dest_mac_addr=dest_nic.mac_addr,
                src_mac_addr=self.mac_addr,
                data_type=0,
                data_bytes=b[start:end],
            )

            self.hub.send(src_nic=self, data_bytes=f.get_bytes())

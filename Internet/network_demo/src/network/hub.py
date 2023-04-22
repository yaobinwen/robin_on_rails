import copy
import logging

from network.device import NICConnectingDevice
from network.frame import Frame
from network.osi_layers import OSILayer1


logger = logging.getLogger("hub")


class Hub(OSILayer1, NICConnectingDevice):
    """A network hub.
    """
    def __repr__(self):
        return (
            "Hub<"
            f"name={self.name},"
            f"port_num={self.port_num},"
            ">"
        )

    def send(self, src_nic: "NIC", data_bytes: bytes):
        """Send the data to this hub.
        """
        logger.info(f"'{self}' is broadcasting data.")

        l = len(data_bytes)

        # A hub is a dumb device. It works in Layer 1 and doesn't know the MAC
        # addresses. It simply broadcasts to all the connected NICs except the
        # NIC that sends the data.
        for mac_addr, c in self.cards.items():
            if src_nic.mac_addr == mac_addr:
                # Do not send to the NIC that sends this frame.
                logger.info(
                    f"'{c}' is the NIC that sends the data. Skip it."
                )
                continue

            logger.info(f"'{self}' is sending {l} bytes(s) of data to '{c}'.")
            c.recv(copy.deepcopy(data_bytes))
            logger.info(f"'{self}' sent {l} byte(s) of data to '{c}'.")

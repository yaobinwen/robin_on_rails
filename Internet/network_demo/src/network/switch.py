import copy
import logging

from network.device import NICConnectingDevice
from network.frame import Frame
from network.osi_layers import OSILayer2


logger = logging.getLogger("switch")


class Switch(OSILayer2, NICConnectingDevice):
    """A network switch.
    """
    def __repr__(self):
        return (
            "Switch<"
            f"name={self.name},"
            f"port_num={self.port_num},"
            ">"
        )

    def send(self, src_nic: "NIC", data_bytes: bytes):
        """Send the data to the destination NIC.
        """
        logger.info(f"'{self}' is sending data.")

        # A switch understands the MAC addresses. This makes a switch a layer-2
        # device.
        f = Frame.from_bytes(data_bytes)

        if f.dest_mac_addr not in self.cards:
            logger.warn(
                f"The NIC of '{f.dest_mac_addr}' "
                "is not connected to this switch."
            )
            return

        c = self.cards.get(f.dest_mac_addr)

        logger.info(f"'{self}' is sending data to the destination NIC '{c}'.")
        c.recv(copy.deepcopy(data_bytes))

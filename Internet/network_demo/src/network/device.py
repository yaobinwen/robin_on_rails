import abc
from typing import List


class Device(object):
    """The base class for all the hardware devices.
    """

    def __init__(self, name: str):
        """Initializer.

        Args:
            name: The name of this device.
        """
        assert isinstance(name, str)

        self._name = name

    @property
    def name(self):
        """Return the name of this device.
        """
        return self._name


class NICConnectingDevice(Device):
    """The base class for the devices that connect NICs, e.g., hubs, switches.
    """
    def __init__(self, name: str, port_num: int=2):
        """Initialize.

        Args:
            name: Name of this device.
            port_num: Number of ports on the device.
        """
        super().__init__(name=name)

        self.cards = {}
        self.port_num = port_num

        if port_num < 2:
            raise ValueError(f"'{self}' must have at least 2 ports.")

    @abc.abstractmethod
    def __repr__(self):
        pass

    def available(self):
        return self.port_num - len(self.cards.keys())

    def connect(self, cards: List["NIC"]):
        """Connect network interface cards.

        Args:
            cards: The NICs to connect to the device.
        """
        for c in cards:
            if c.mac_addr in self.cards:
                continue

            if self.available() == 0:
                raise RuntimeError(
                    f"'{self}' has no available port for any new NIC."
                )

            self.cards[c.mac_addr] = c
            c.connect(hub=self)

    def disconnect(self, cards: List["NIC"]):
        """Disconnect network interface cards.

        Args:
            cards: The NICs to disconnect from the device.
        """
        for c in cards:
            if c.mac_addr not in self.cards:
                continue

            del self.cards[c.mac_addr]

    @abc.abstractmethod
    def send(self, src_nic: "NIC", data_bytes: bytes):
        """Send the data to this device.
        """
        pass

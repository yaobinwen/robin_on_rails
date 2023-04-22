import re


class MACAddr(object):
    """Media Access Control (MAC) address.
    """

    LENGTH = 12

    def __init__(self, addr: str):
        if not isinstance(addr, str):
            raise TypeError(
                f"'addr' must be '{str.__name__}' "
                f"but now is '{addr.__class__.__name__}'."
            )

        regex = re.compile("^[0-9a-fA-F]{%d}$" % MACAddr.LENGTH)
        if regex.match(addr) is None:
            raise ValueError(
                f"The address '{addr}' does not match "
                f"the pattern '{regex.pattern}'."
            )

        self.addr = addr

    def __eq__(self, other):
        return self.addr.upper() == other.addr.upper()

    def __hash__(self):
        return hash(self.addr.upper())

    def __ne__(self, other):
        return self.addr.upper() != other.addr.upper()

    def __repr__(self):
        return (
            "MACAddr<"
            f"addr='{self.addr.upper()}',"
            ">"
        )


# Alias: MAC addresses are also known as physical addresses.
PhysicalAddress = MACAddr

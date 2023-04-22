#!/usr/bin/python3

import logging

from network.frame import Frame
from network.hub import Hub
from network.mac_addr import MACAddr
from network.nic import NIC


logger = logging.getLogger("demo_simple_hub")


def main():
    # Set up the logging.
    logging.basicConfig(
        datefmt="%FT%H:%M:%S",
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
        level=logging.DEBUG,
    )

    c1 = NIC(mac_addr=MACAddr("11112222aaaa"), name="NIC-1")
    c2 = NIC(mac_addr=MACAddr("11112222bbbb"), name="NIC-2")
    c3 = NIC(mac_addr=MACAddr("11112222cccc"), name="NIC-3")

    hub = Hub(name="hub", port_num=3)

    # Connect the NICs to the hub.
    hub.connect([c1, c2, c3])

    data = "abc"
    logger.info(f"'{c1}' is sending data ({len(data)} byte(s)) to '{c3}'.")
    c1.send(data=data, encoding="UTF-8", dest_nic=c3)
    logger.info(f"'{c1}' sent data ({len(data)} to '{c3}'.")

    data = (
        "a" * Frame.DATA_BYTES_MAX_LENGTH + "A" * Frame.DATA_BYTES_MAX_LENGTH +
        '+' * 10
    )
    logger.info(f"'{c1}' is sending data ({len(data)} to '{c2}'.")
    c1.send(data=data, encoding="UTF-8", dest_nic=c2)
    logger.info(f"'{c1}' sent data ({len(data)} to '{c2}'.")


if __name__ == "__main__":
    main()

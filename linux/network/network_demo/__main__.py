#!/usr/bin/env python3


import network_demo.cable as nw_cable
import network_demo.host as nw_host
import network_demo.router as nw_router


def demo_01():
    # Instantiate all the hardware.
    cable1 = nw_cable.Cable()
    cable2 = nw_cable.Cable()
    host1 = nw_host.Host(1)
    host2 = nw_host.Host(1)
    router1 = nw_router.Router(8)

    # Connect the network.
    # Connect the host1 and router1.
    host1.plug(cable1, 0)
    router1.plug(cable1, 0)
    # Connect the host2 and router1.
    host2.plug(cable2, 0)
    router1.plug(cable2, 1)


    # TODO(ywen): Implement me!
    pass


def main():
    demo_01()


if __name__ == '__main__':
    main()

import unittest

from network.hub import Hub
from network.mac_addr import MACAddr
from network.nic import NIC


class TestHub(unittest.TestCase):
    def test_init(self):
        for n in (-1, 0, 1):
            self.assertRaises(ValueError, Hub, name="hub", port_num=n)

        hub = Hub(name="hub", port_num=3)
        self.assertEqual(hub.name, "hub")
        self.assertEqual(hub.port_num, 3)
        self.assertEqual(hub.available(), 3)

    def test_repr(self):
        hub = Hub(name="hub", port_num=5)
        self.assertEqual(repr(hub), "Hub<name=hub,port_num=5,>")

    def test_connect(self):
        c1 = NIC(mac_addr=MACAddr("11112222aaaa"), name="NIC-1")
        c2 = NIC(mac_addr=MACAddr("11112222bbbb"), name="NIC-2")
        c3 = NIC(mac_addr=MACAddr("11112222cccc"), name="NIC-3")
        c4 = NIC(mac_addr=MACAddr("11112222dddd"), name="NIC-4")

        hub = Hub(name="hub", port_num=3)
        self.assertEqual(hub.available(), 3)

        hub.connect([c1])
        self.assertEqual(hub.available(), 2)
        self.assertIn(c1.mac_addr, hub.cards)
        self.assertIsNotNone(c1.hub)

        hub.connect([c1, c2])
        self.assertEqual(hub.available(), 1)
        for c in (c1, c2):
            self.assertIn(c.mac_addr, hub.cards)
            self.assertIsNotNone(c.hub)

        hub.connect([c3])
        self.assertEqual(hub.available(), 0)
        for c in [c1, c2, c3]:
            self.assertIn(c.mac_addr, hub.cards)

        self.assertRaises(RuntimeError, hub.connect, cards=[c2, c4])

    def test_disconnect(self):
        c1 = NIC(mac_addr=MACAddr("11112222aaaa"), name="NIC-1")
        c2 = NIC(mac_addr=MACAddr("11112222bbbb"), name="NIC-2")
        c3 = NIC(mac_addr=MACAddr("11112222cccc"), name="NIC-3")

        hub = Hub(name="hub", port_num=3)
        hub.connect([c1, c2, c3])

        hub.disconnect([c1])
        self.assertNotIn(c1.mac_addr, hub.cards)

        hub.disconnect([c1, c2, c3])
        self.assertEqual(hub.available(), 3)


if __name__ == "__main__":
    unittest.main()

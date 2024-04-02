# IPv6

## 1. Address Representation

1). Both lowercase and uppercase are equivalent, but [RFC 5952: A Recommendation for IPv6 Address Text Representation](https://datatracker.ietf.org/doc/html/rfc5952) recommends **lowercase**.

2). `hextet`: The **unofficial** term for a section of **four** hexadecimal values. For example, in the IPv6 address `fe80:0000:0000:0000:b8a2:ee12:4467:03e8`, `fe80`, `0000`, `4467` can all be called "hextet" (unofficially).

3). Shorter forms: Use `fe80:0000:0000:0000:b8a2:ee12:4467:03e8` as an example:
- a).  First, we can suppress all leading 0s. So `03e8` is the same as `3e8` and all those strings of 0000 can be written simply as 0. So the address can be `fe80:0:0:0:b8a2:ee12:4467:3e8`.
- b). You can replace two or more blocks of `0000` in succession with `::`. So the address can be written as `fe80::b8a2:ee12:4467:3e8`.
  - i). If the address has more than one string of zeroes, you can only replace the **longest one** with the double colon. So an address like `fe80:0000:0000:0a13:0000:0000:0000:001a` would be rewritten as `fe80:0:0:a13::1a`.
  - ii). If the strings of zeros are of equal length, the first string should use the double colon (`::`) notation.

---

## Others


Link-local addresses are generated automatically. They're only valid on the local LAN segment. When my computer joins the network, it creates a link-local address for itself and uses that for communicating with the nearest router, negotiating its IP address and so forth. It never uses the link-local address for sending packets to other networks. Before it can do that, the computer needs a global address.

IPv6 has two different methods for automatically configuring devices:
- stateful autoconfiguration using DHCPv6, and
- stateless autoconfiguration, known as Neighbor Discovery Protocol (NDP).

## How Neighbor Discovery Protocol works

NDP is a simple protocol that IPv6 devices use to figure out what address they should use, as well as how to find the router that will forward their packets to the rest of the world. It's based on the IPv6 version of ICMP, a protocol that should be familiar to IPv4 users who use it with the common ping utility.

Network devices can send an IPv6 neighbor discovery **router solicitation (RS)** packet to the default multicast address that all routers always listen to. Any router on the segment that receives it will respond with an IPv6 neighbor discovery **router advertisement (RA)**. The RA tells it how to forward packets to destinations that aren't on the same segment.

Similarly, IPv6 uses the IPv6 neighbor discovery **neighbor solicitation (NS)** multicast to discover the MAC address of another host on the same segment. The neighboring device responds with a **neighbor advertisement (NA)** packet. This is the IPv6 analog to Address Resolution Protocol (ARP).

## How addressing works with stateless autoconfiguration

IPv6 has a built-in feature called stateless autoconfiguration that allows new devices to join a network and start working with zero information and without a DHCP server. In general, though, you'll probably want to use DHCP as well.

When a new device joins the network, the first thing it does is generate a link-local address from the range mentioned above. It always sets the first part of the address to fe80. Then it sets the last 64 bits according to a standard set of rules, defined uniquely from the device's Ethernet MAC address. All of the other digits in the address are zeroes.

That address is supposed to be unique, but there's always a chance that something went wrong. So the next thing it does is use a neighbor solicitation message to try to find any other devices on the same segment that might already be using the address. If the address isn't in use, then it's safe to start using it. However, the protocol doesn't actually define what the device should do next if it does encounter a conflict.

Then the device uses its link-local address to contact the router for the segment. It sends a router solicitation message and waits for a router advertisement in response that will tell it how to reach the rest of the world.

But it still can't reach the rest of the world because it doesn't have a globally unique address. To automatically generate the global IPv6 address, the device takes the first 64 bits of the router's address, which it learned from the router advertisement, and the last 64 bits from its own link-local address.

This addressing is important to remember. When you use stateless autoconfiguration, devices assume they're operating on a /64 subnet. If you need to use a different subnet masking scheme, you must configure the addressing either statically or via DHCPv6.

## IPv6 multicasts are more efficient

Another important way that IPv6 differs from IPv4 is its use of multicast. In IPv4, we use broadcast messages quite frequently. When a new host needs to find out its IP address, it sends out a DHCP broadcast. When it needs to communicate with a destination IP address but doesn't know the corresponding Ethernet MAC address, it sends out an ARP broadcast.

The trouble with broadcasts is that every device on the same network has to pay attention to every broadcast packet. Those broadcast packets are received by the host's NIC and passed up the protocol stack, taking up CPU and memory resources on every system. Broadcasts are not a very efficient way to use the network. So IPv6 uses **multicast** instead.

In multicast, the sending device transmits a packet with a special IP address and a special Ethernet MAC address. Any other device on the network that's taking part in that particular multicast protocol picks it up and processes it, but everybody else can ignore it.

Making things even better, if the switches are configured correctly, they can keep track of which devices are interested in which multicast protocols. Then the switch only forwards those multicast packets to the interested devices. This helps reduce network congestion.

## IPv6 temporary addresses

An IPv6 temporary address includes a randomly generated 64-bit number as the interface ID instead of an interface's MAC address. You can use temporary addresses for any interface on an IPv6 node that you want to keep anonymous. For example, you might want to use temporary addresses for the interfaces of a host that needs to access public web servers. Temporary addresses implement IPv6 privacy enhancements. These enhancements are described in [RFC 8981: Temporary Address Extensions for Stateless Address Autoconfiguration in IPv6](https://datatracker.ietf.org/doc/html/rfc8981).

On Ubuntu, the generation of IPv6 temporary addresses is managed by these `sysctl` settings (in `/etc/sysctl.d/10-ipv6-privacy.conf` or a similar file):

```conf
# IPv6 typically uses a device's MAC address when choosing an IPv6 address
# to use in autoconfiguration. Privacy extensions allow using a randomly
# generated IPv6 address, which increases privacy.
#
# Acceptable values:
#    0 - don't use privacy extensions.
#    1 - generate privacy addresses
#    2 - prefer privacy addresses and use them over the normal addresses.
net.ipv6.conf.all.use_tempaddr = 2
net.ipv6.conf.default.use_tempaddr = 2
```

## TODOs

- [IPv6 Router Advertisements](https://docs.netgate.com/pfsense/en/latest/services/dhcp/ipv6-ra.html)
- IPV6: Stateful vs. Stateless:
  - https://www.baeldung.com/cs/ipv6-stateful-vs-stateless
  - https://www.networkacademy.io/ccna/ipv6
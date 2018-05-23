# Unix and Linux Administration Handbook (4th Edition)

## Overview

(TODO)

## Chapter 14: TCP/IP Networking

### 14.3 PACKET ADDRESSING

---

> Each of a host’s network interfaces usually has one link-layer MAC address that distinguishes it from other machines on the physical network, plus one or more IP addresses that identify the interface on the global Internet.

- **Q**: How are the MAC addresses used in networking? How does a machine "publish" its network interface MAC address to the environment?

---

> For example, Ethernet devices are assigned a unique 6-byte hardware address at the time of manufacture. These addresses are traditionally written as a series of 2-digit hex bytes separated by colons; for example, 00:50:8D:9A:3B:DF.

- **Q**: Is there any authority in charge of the MAC address assignment? Or do the hardware manufacturers randomly create the addresses and use them?
  - According to [this answer](https://superuser.com/a/968346/224429):
    - In general, IEEE is in charge of MAC address assignment, but they don't guarantee the uniqueness of every network device's MAC address.
    - **Devices are not uniquely identified by their MAC addresses.**
    - The MAC addresses can be duplicated for various reasons (see the details in the answer, as well as the paragraphs in the book that follow, for some possible reasons).

---

> The mapping from IP addresses to hardware addresses is implemented at the **link layer** of the **TCP/IP model**. On networks such as Ethernet that support broadcasting, senders use the **ARP protocol to discover mappings without assistance** from a system administrator. In IPv6, **an interface’s MAC address can be used as part of the IP address**, making the translation between IP and hardware addressing virtually automatic.

- Note that it emphasizes the link layer of the **TCP/IP model**.
- **Q:** What is the role of the **link layer** in a network? It looks like the link layer **somehow** connects all the hardware that appears in the network.
  - [This Wikipedia article](https://en.wikipedia.org/wiki/Link_layer) explains the **link layer** in the **TCP/IP model**.
- "14.6 ARP: THE ADDRESS RESOLUTION PROTOCOL" says: "`ARP, the Address Resolution Protocol, discovers the hardware address associated with a particular IP address.`"

---

> Under UNIX and Linux, this mapping [hostname to IP address] can be set up in several ways, ranging from a static file (/etc/hosts) to the [LDAP database system](https://en.wikipedia.org/wiki/Lightweight_Directory_Access_Protocol) to DNS, the world-wide Domain Name System.

---

> TCP and UDP extend IP addresses with a con- cept known as a port, a 16-bit number that supplements an IP address to specify a particular communication channel.

- You can find a full list of assigned ports at [Service Name and Transport Protocol Port Number Registry](https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml).

### 14.4 IP ADDRESSES: THE GORY DETAILS

---

> ... you can now reassign part of the host portion to the network portion ...The 1s must be leftmost and contiguous. At least eight bits must be allocated to the network part and at least two bits to the host part. Ergo, there are really only **22 possible** values for an IPv4 netmask.

- There are only **22** possible values for an IPv4 netmask because of the following reasons:
  - Firstly, an IPv4 address has 32 bits in total: xxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx
  - Secondly, 8 bits are reserved for the network part, leaving 24 bits for the host part. Note that the "network" in "network part" does not refer to the concept of "subnet": (nnnnnnnn)(network part).xxxxxxxx.xxxxxxxx.xxxxxxxx
  - Thirdly, 2 bits must be used for the host part, leaving only 22 bits for the subnet mask: (nnnnnnnn)(network part).xxxxxxxx.xxxxxxxx.xxxxxx(hh)(host part)
  - Finally, the 1st must be leftmost and contiguous, so the 22 possible subnet mask values are:
    - 1). (nnnnnnnn)(network part).10000000.00000000.000000(hh)(host part)
    - 2). (nnnnnnnn)(network part).11000000.00000000.000000(hh)(host part)
    - 3). (nnnnnnnn)(network part).11100000.00000000.000000(hh)(host part)
    - ...
    - 20). (nnnnnnnn)(network part).11111111.11111111.111100(hh)(host part)
    - 21). (nnnnnnnn)(network part).11111111.11111111.111110(hh)(host part)
    - 22). (nnnnnnnn)(network part).11111111.11111111.111111(hh)(host part)

---

> For example, the network address 128.138.243.0/26 refers to the first of four networks whose first bytes are 128.138.243. The other three net- works have 64, 128, and 192 as their fourth bytes.

- There are "four" networks because of the following reasons:
  - The prefix "/26" indicates that 26 bits are used as the subnet mask. In other words, 2 leftmost bits from the last octet are used as the "network" part of the IP addresses.
  - If we keep the first three bytes the same as "128.138.243", then there are only 4 possible values for those 2 bits borrowed from the last octet: 00(0), 01(1), 10(2), and 11(3).
  - Because these 2 bits are the lelftmost ones, putting them back to the octet makes it, respectively, the following values ("h" indicates a host bit):
    - 00hhhhhh(0), which is the one shown in "128.138.243.0/26".
    - 01hhhhhh(64)
    - 10hhhhhh(128)
    - 11hhhhhh(192)

---

TODO: Summarize `Private addresses and network address translation (NAT)`
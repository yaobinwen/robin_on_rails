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

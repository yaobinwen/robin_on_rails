# Notes

## >> A. Primary References

This section lists the references that I use primarily. Usually they are books. The references in this section are referred to as `[A.n]`. The other secondary references (usually online articles that focus on various topics) are listed in each section.

- [1] [CompTIA Network+ Certification All-in-One Exam Guide, Seventh Edition (Exam N10-007)](https://www.amazon.com/CompTIA-Network-Certification-Seventh-N10-007/dp/1260122387/)
- [2] [Understanding Linux Network Internals](https://www.amazon.com/Understanding-Linux-Network-Internals-Networking/dp/0596002556)

---

## >> Network Models

There are two prevailing network models: **OSI model** (7 layers) and **TCP/IP model** (4 layers):

<table border="1px solid black" border-collapse="collapse">
  <caption>Model Layer Comparison</caption>
  <tbody>
    <tr>
      <th>Layer</th>
      <th>OSI Model</th>
      <th>TCP/IP Model</th>
      <th>Protocol Data Unit (PDU)</th>
      <th>Function</th>
    </tr>
    <tr style="background:#d8ec9b;">
      <td>7</td>
      <td>Application</td>
      <td rowspan=3>Application</td>
      <td rowspan=3>Data (payload)</td>
      <td><p>High-level APIs, including resource sharing, remote file access</p></td>
    </tr>
    <tr style="background:#d8ec9b;">
      <td>6</td>
      <td>Presentation</td>
      <td><p>Translation of data between a networking service and an application; including character encoding, data compression and encryption/decryption</p></td>
    </tr>
    <tr style="background:#d8ec9b;">
      <td>5</td>
      <td>Session</td>
      <td><p>Managing communication sessions, i.e., continuous exchange of information in the form of multiple back-and-forth transmissions between two nodes</p></td>
    </tr>
    <tr style="background:#e7ed9c;">
      <td>4</td>
      <td>Transport</td>
      <td>Transport</td>
      <td>Segment, Datagram</td>
      <td><p>Reliable transmission of data segments between points on a network, including segmentation, acknowledgement and multiplexing</p></td>
    </tr>
    <tr style="background:#eddc9c;">
      <td>3</td>
      <td>Network</td>
      <td>Internet</td>
      <td>Packet</td>
      <td><p>Structuring and managing a multi-node network, including addressing, routing and traffic control</p></td>
    </tr>
    <tr style="background:#e9c189;">
      <td>2</td>
      <td>Data Link</td>
      <td rowspan=2>Link</td>
      <td>Frame</td>
      <td><p>Reliable transmission of data frames between two nodes connected by a physical layer</p></td>
    </tr>
    <tr style="background:#e9988a;">
      <td>1</td>
      <td>Physical</td>
      <td>Bit, Symbol</td>
      <td><p>Transmission and reception of raw bit streams over a physical medium</p></td>
    </tr>
  </tbody>
</table>

_References_:
- [1] [Wikipedia: Open Systems Interconnection model (OSI model)](https://en.wikipedia.org/wiki/OSI_model)
- [2] [Wikipedia: Internet protocol suite](https://en.wikipedia.org/wiki/Internet_protocol_suite)

---

## >> Layer 1 & 2: Physical & Data Link

### Network Interface Card (NIC)

A **_network interface card (NIC)_** serves as the interface between the computer and the network.
- On older systems, a _NIC_ truly was a separate card that snapped into an expansion slot, which is why they were called network interface _cards_.
- They are still called NICs even though today they are built into the motherboard.

### MAC Address

Every NIC has a unique identifier called **_media access control address_**, or **_MAC address_**.
- No two NICs share the same MAC address.
- IEEE is responsible in allocating the MAC addresses to different NIC manufacturers.
- A MAC address consists of 48 bits, or 12 hex digits:
  - The first 6 hex digits are _Organizationally Unique Identifier (OUI)_.
  - The last 6 hex digits are _device IDs_.
- Also called _physical address_.
- Also called _EUI-48_ (_Extended Unique Identifier_).

### Frame

Data Link layer sends and receives data in **_frames_**. A frame consists of the following fields:
- Recipient's MAC address
- Sender's MAC address
- Type, or [`EtherType`](https://en.wikipedia.org/wiki/EtherType): indicates which protocol is encapsulated in the payload of the frame so the receiving end knows how to process the data. See [EtherType:Values](https://en.wikipedia.org/wiki/EtherType#Values) for possible values.

### Repeater, Bridge, Hub, Switch, Router, Gateway

See Page 14-15 in [A.1] and the introduction section of Chapter 14 in [A.2]:

A **_repeater_** makes an except copy of the receiving frame and send a copy of the original frame out of all the connected ports except the port on which the frame originates. _Every_ frame sent on a network is received by _every_ NIC, but **only the NIC with the matching MAC address will process the frame**.
- A repeater does not have any knowledge of protocols. It simply copies data bit by bit.
- A repeater that has multiple ports is called a **_hub_**.

A **_bridge_** understands **link layer** protocols and therefore copies data frame by frame, instead of bit by bit. It does not send all incoming frames to all network devices that are connected to it, but sends the frame **only to the interface associated with the destination MAC address**.

Regarding the difference between _bridges_ and _switches_ (see [A.2], Chapter 14):
- A _bridge_ is also called a **_switch_**.
- The term _bridge_ is mainly used in the documentation (such as the IEEE specifications).
- The term _switch_ is mainly used when people talk about and refer to the device. However, when a _switch_ has only **two ports**, people may also use the term _bridge_ to refer to it.

A **_router_** understands L3 network protocols such as IP, and forwards ingress packets based on a routing table. The term **_gateway_**, which was introduced before router, is also commonly used to refer to a router. But they are not exactly the same:
- They both help forward data between networks.
- However, a router routes data between the networks based on the same protocol, while a gateway routes data between dissimilar networks so a gateway needs to **translate from one protocol to another**. As explained in [this article](https://internetofthingsagenda.techtarget.com/definition/gateway):

> A gateway is a network node that connects two networks using different protocols together. While a bridge is used to join two similar types of networks, a gateway is used to join two dissimilar networks.

---

## >> Collision Domain and Broadcast Domain

A **collision domain** is a network segment connected by a **shared medium** or through **repeaters** where simultaneous data transmissions collide with one another. A network collision occurs when more than one device attempts to send a packet on a network segment at the same time. The packets collide and both devices must send the packets again, which **reduces network efficiency**.

By contrast, each port on a **bridge**, a **switch** or a **router** is in a **separate** collision domain.

The following picture (quoted from [2]) illustrates 6 different collision domains:

![Collision domain (see "./files/collision_domains.jpg")](https://study-ccna.com/wp-content/images/collision_domains.jpg)

A **broadcast domain** is the domain in which a broadcast is forwarded. A broadcast domain contains all devices that can reach each other at the data link layer (OSI layer 2) by using broadcast. All ports on a hub or a switch are by default in the same broadcast domain. All ports on a router are in the different broadcast domains and routers don’t forward broadcasts from one broadcast domain to another.

The following picture (quoted from [2]) illustrates 3 different broadcast domains:

![Broadcast domain (see "./files/broadcast_domains.jpg")](https://study-ccna.com/wp-content/images/broadcast_domains.jpg)

References:
- [1] [Wikipedia: Collision domain](https://en.wikipedia.org/wiki/Collision_domain)
- [2] [Collision & broadcast domain](https://study-ccna.com/collision-broadcast-domain/)

---

## >> Genmask

Quoted from [this answer](https://serverfault.com/a/696992/125167):

> **Genmask**: The netmask for the destination net; 255.255.255.255 for a host destination and 0.0.0.0 for the default route.

Someone else in the same answer also mentioned that:

> It's just odd that the output of route seems to be the only place the actual term "genmask" is used. Everywhere else it's "netmask." E.g. in my system's man files, "netmask" shows up 207 times, but "genmask" is only mentioned in the route man page.

---

## >> Predictable Network Interface Names

See [Predictable Network Interface Names](https://systemd.io/PREDICTABLE_INTERFACE_NAMES/).

> Starting with v197 systemd/udev will automatically assign predictable, stable network interface names for all local Ethernet, WLAN and WWAN interfaces.
>
> The classic naming scheme for network interfaces applied by the kernel is to simply assign names beginning with eth0, eth1, ... to all interfaces as they are probed by the drivers. As the driver probing is generally not predictable for modern technology this means that as soon as multiple network interfaces are available the assignment of the names eth0, eth1 and so on is generally not fixed anymore and it might very well happen that eth0 on one boot ends up being eth1 on the next. This can have serious security implications, for example in firewall rules which are coded for certain naming schemes, and which are hence very sensitive to unpredictable changing names.

Also see [_systemd.net-naming-scheme - Network device naming schemes_](https://www.freedesktop.org/software/systemd/man/systemd.net-naming-scheme.html)

---

## >> Debian: `/etc/nsswitch.conf`

See [nsswitch.conf(5)](https://man7.org/linux/man-pages/man5/nsswitch.conf.5.html):

> The Name Service Switch (NSS) configuration file, `/etc/nsswitch.conf`, is used by the GNU C Library and certain other applications to determine the sources from which to obtain name-service information in a range of categories, and in what order.  Each category of information is identified by a database name.

---

## >> `net-tools` & `iproute2`

`net-tools` suite is being abandoned because the developers haven't updated it. Use `iproute2` instead, if possible.

---

## >> Debian: `/etc/network/interfaces`

The network configuration file.

---

## >> `networking.service` and `NetworkManager.service`

```
$ service networking status
● networking.service - Raise network interfaces
   Loaded: loaded (/lib/systemd/system/networking.service; enabled; vendor preset: enabled)
   Active: active (exited) since Mon 2020-11-09 18:06:38 EST; 1 weeks 4 days ago
     Docs: man:interfaces(5)
 Main PID: 1068 (code=exited, status=0/SUCCESS)
    Tasks: 0 (limit: 4915)
   CGroup: /system.slice/networking.service
```

```
$ service NetworkManager status
● NetworkManager.service - Network Manager
   Loaded: loaded (/lib/systemd/system/NetworkManager.service; enabled; vendor preset: enabled)
   Active: active (running) since Mon 2020-11-09 18:06:45 EST; 1 weeks 4 days ago
     Docs: man:NetworkManager(8)
 Main PID: 1642 (NetworkManager)
    Tasks: 5 (limit: 4915)
   CGroup: /system.slice/NetworkManager.service
           ├─ 1642 /usr/sbin/NetworkManager --no-daemon
           ├─10627 /sbin/dhclient -d -q -sf /usr/lib/NetworkManager/nm-dhcp-helper -pf /run/dhclient-wlp3s0.pid -lf /var/lib/NetworkManager/dhclient-ee9ba2c2-bd6b-472f-a818-feeacb0cd593-wlp3s0.lease -cf /
           └─13547 /sbin/dhclient -d -q -6 -N -sf /usr/lib/NetworkManager/nm-dhcp-helper -pf /run/dhclient6-wlp3s0.pid -lf /var/lib/NetworkManager/dhclient6-ee9ba2c2-bd6b-472f-a818-feeacb0cd593-wlp3s0.lea
```

---

## >> `/etc/gai.conf`

This is the [`getaddrinfo(3)`](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html) configuration file. According to its [manual page](https://man7.org/linux/man-pages/man5/gai.conf.5.html):

> A call to `getaddrinfo(3)` might return multiple answers.  According to
> RFC 3484 these answers must be sorted so that the answer with the
> highest success rate is first in the list.  The RFC provides an
> algorithm for the sorting.  The static rules are not always adequate,
> though.  For this reason, the RFC also requires that system
> administrators should have the possibility to dynamically change the
> sorting.  For the glibc implementation, this can be achieved with the
> `/etc/gai.conf` file.

---

## >> File Servers

| File Server | Scenario |
|:-----------:|:---------|
| Network File System (NFS) | Linux-based environment |
| Samba | Mixed environment |
| SSHFS | Secure transfer; only need to access the remote FS occasionally [1] |

[1] Because SSHFS is easy to use and doesn't need external configuration, if you just want to access a remote file server once or occasionally, with SSHFS you don't have to bother to create an configuratio file as you need for NFS and Samba.

---

## >> Monitoring: load average

`cat /proc/loadavg` produces output like the following:

```
0.49 0.77 0.94 1/2698 17529
```

- The first three numbers represent the system's load during a period of 1, 5, and 15 minutes respectively.
- The load that's being referred to represents the number of processes that are waiting on, or currently utilizing, the CPU during each timeframe.

> As a general rule of thumb, it's a good idea to record a baseline of your systems when they are under their normal, expected load. Each system on your network will have a designated purpose and each will have a certain load you can reasonably expect your system to face at any one time. If the systems load average dips too far below or climbs higher than the baseline, then you would want to take a look and find out what's going on. If the load reaches a level where there are more processes than you have cores to handle, that's cause for alarm. (_Mastering Linux Network Administration_)

---

## >> Monitoring: `htop`, `iotop`, `ncdu`

`htop` is a good alternative to `top`.

`iotop` shows the "top" for I/O.

`ncdu` is a good alternative to `du`.

---

## >> IPv6: don't bother unless having a good reason

> you might be tempted to roll-out IPv6 addresses over IPv4 within your network. However, my suggestion is unless you have a very good reason to do so, don't bother. The depletion of IPv4 addresses only affects the public Internet, not your internal network. While you can certainly roll-out IPv6 internally, there's no benefit to doing so. Given that IPv4 has over 4 billion addresses available, you would need quite a few devices in order to justify IPv6. On the other hand, IPv6 is certainly useful (and will eventually be required) for telecoms. It's also useful for those of you who are studying Cisco exams, as understanding of this topic is required. But for the purposes of this book and setting up Linux networks, IPv6 doesn't justify the administration overhead. (_Mastering Linux Network Administration_)

---

## >> Static IP Addresses vs Static Leases

You must configure the static IP addresses on the machines that use them. You must be able to find where those machines are.

The static leases are configured on the DHCP servers so it's a central place for you to manage them.

Therefore, static leases may be more convenient for administration.

---

## >> TUN/TAP

According to [1], `TUN` and `TAP` are kernel **virtual** network devices:

| Device | Type | Purpose | Layer | Data | Used for |
|:------:|:-----:|:-------:|:-----:|:----:|:-------:|
| TAP | Virtual | Tunneling | Link | Ethernet frames | Bridging |
| TUN | Virtual | Tunneling | Network | IP packets | Routing |

References:
- [1] [Wikipedia: TUN/TAP](https://en.wikipedia.org/wiki/TUN/TAP)
- [2] [Tun/Tap interface tutorial](https://backreference.org/2010/03/26/tuntap-interface-tutorial/)

---

## >> Special IP Addresses

| Address block | Address range | Number of addresses | Scope | Description |
|:--------------|:--------------|:-------------------:|:------|:------------|
| 0.0.0.0/8 | 0.0.0.0–0.255.255.255 | 16777216 | Software | Current network[3] (only valid as source address). |
| 10.0.0.0/8 | 10.0.0.0–10.255.255.255 | 16777216 | Private network | Used for local communications within a private network.[4] |
| 100.64.0.0/10 | 100.64.0.0–100.127.255.255 | 4194304 | Private network | Shared address space[5] for communications between a service provider and its subscribers when using a carrier-grade NAT. |
| 127.0.0.0/8 | 127.0.0.0–127.255.255.255 | 16777216 | Host | Used for loopback addresses to the local host.[3] |
| 169.254.0.0/16 | 169.254.0.0–169.254.255.255 | 65536 | Subnet | Used for link-local addresses[6] between two hosts on a single link when no IP address is otherwise specified, such as would have normally been retrieved from a DHCP server. |
| 172.16.0.0/12 | 172.16.0.0–172.31.255.255 | 1048576 | Private network | Used for local communications within a private network.[4] |
| 192.0.0.0/24 | 192.0.0.0–192.0.0.255 | 256 | Private network | IETF Protocol Assignments.[3] |
| 192.0.2.0/24 | 192.0.2.0–192.0.2.255 | 256 | Documentation | Assigned as TEST-NET-1, documentation and examples.[7] |
| 192.88.99.0/24 | 192.88.99.0–192.88.99.255 | 256 | Internet | Reserved.[8] Formerly used for IPv6 to IPv4 relay[9] (included IPv6 address block 2002::/16). |
| 192.168.0.0/16 | 192.168.0.0–192.168.255.255 | 65536 | Private network | Used for local communications within a private network.[4] |
| 198.18.0.0/15 | 198.18.0.0–198.19.255.255 | 131072 | Private network | Used for benchmark testing of inter-network communications between two separate subnets.[10] |
| 198.51.100.0/24 | 198.51.100.0–198.51.100.255 | 256 | Documentation | Assigned as TEST-NET-2, documentation and examples.[7] |
| 203.0.113.0/24 | 203.0.113.0–203.0.113.255 | 256 | Documentation | Assigned as TEST-NET-3, documentation and examples.[7] |
| 224.0.0.0/4 | 224.0.0.0–239.255.255.255 | 268435456 | Internet | In use for IP multicast.[11] (Former Class D network.) |
| 233.252.0.0/24 | 233.252.0.0-233.252.0.255 | 256 | Documentation | Assigned as MCAST-TEST-NET, documentation and examples.[11][12] |
| 240.0.0.0/4 | 240.0.0.0–255.255.255.254 | 268435455 | Internet | Reserved for future use.[13] (Former Class E network.) |
| 255.255.255.255/32 | 255.255.255.255 | 1 | Subnet | Reserved for the "limited broadcast" destination address.[3][14] |

---

## >> `0.0.0.0/0` and `::/0`

In the context of routing tables, a network destination of 0.0.0.0 is used with a network mask of 0 to depict the **default route** as a destination subnet. This destination is expressed as 0.0.0.0/0 in CIDR notation. It matches all addresses in the IPv4 address space and is present on most hosts, directed towards a local router.

In routing tables, 0.0.0.0 can also appear in the gateway column. This indicates that the gateway to reach the corresponding destination subnet is **unspecified**. This generally means that **no intermediate routing hops are necessary because the system is directly connected to the destination**.

`::/0` is the IPv6 version of `0.0.0.0/0` and serves the same purposes.

# Notes

## Bridge (Switch)

A `bridge` is a link-layer device that copies data frame by frame. Unlike a `hub` (or `repeater`) which has no knowledge of network protocols or addresses, a `bridge` understands link-layer addresses (MAC) so it "sends the frame only to the interface associated with the destination MAC address." [1]

According to [2]:

- A `bridge` and a `switch` are essentially the same kind of device.
- `bridge` is mainly used in documentation; `switch` is mainly used in oral discussion.
- A `switch` may be called a `bridge` in oral discussion when this switch has only **two** ports.

References:
- [1] _Mike Meyers: CompTIA Network+ Certification Exam Guide (7e)_, Chapter 1.
- [2] _Understanding Linux Network Internals_, Chapter 14 "Bridging: Concepts"

## Gateway

[gateway](https://internetofthingsagenda.techtarget.com/definition/gateway):

> A gateway is a network node that connects two networks using different protocols together. While a bridge is used to join two similar types of networks, a gateway is used to join two dissimilar networks.

TODO: Update the definition using the book _Understanding Linux Network Internals_.

## Genmask

[This answer](https://serverfault.com/a/696992/125167) quotes:

> **Genmask**: The netmask for the destination net; 255.255.255.255 for a host destination and 0.0.0.0 for the default route.

Someone else in the same answer also mentioned that:

> It's just odd that the output of route seems to be the only place the actual term "genmask" is used. Everywhere else it's "netmask." E.g. in my system's man files, "netmask" shows up 207 times, but "genmask" is only mentioned in the route man page.

## Predictable Network Interface Names

See [Predictable Network Interface Names](https://systemd.io/PREDICTABLE_INTERFACE_NAMES/).

> Starting with v197 systemd/udev will automatically assign predictable, stable network interface names for all local Ethernet, WLAN and WWAN interfaces.
>
> The classic naming scheme for network interfaces applied by the kernel is to simply assign names beginning with eth0, eth1, ... to all interfaces as they are probed by the drivers. As the driver probing is generally not predictable for modern technology this means that as soon as multiple network interfaces are available the assignment of the names eth0, eth1 and so on is generally not fixed anymore and it might very well happen that eth0 on one boot ends up being eth1 on the next. This can have serious security implications, for example in firewall rules which are coded for certain naming schemes, and which are hence very sensitive to unpredictable changing names.

## Debian: `/etc/nsswitch.conf`

See [nsswitch.conf(5)](https://man7.org/linux/man-pages/man5/nsswitch.conf.5.html):

> The Name Service Switch (NSS) configuration file, `/etc/nsswitch.conf`, is used by the GNU C Library and certain other applications to determine the sources from which to obtain name-service information in a range of categories, and in what order.  Each category of information is identified by a database name.

## `net-tools` & `iproute2`

`net-tools` suite is being abandoned because the developers haven't updated it. Use `iproute2` instead, if possible.

## Debian: `/etc/network/interfaces`

The network configuration file.

## `networking.service` and `NetworkManager.service`

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

## `/etc/gai.conf`

This is the [`getaddrinfo(3)`](https://man7.org/linux/man-pages/man3/getaddrinfo.3.html) configuration file. According to its [manual page](https://man7.org/linux/man-pages/man5/gai.conf.5.html):

> A call to `getaddrinfo(3)` might return multiple answers.  According to
> RFC 3484 these answers must be sorted so that the answer with the
> highest success rate is first in the list.  The RFC provides an
> algorithm for the sorting.  The static rules are not always adequate,
> though.  For this reason, the RFC also requires that system
> administrators should have the possibility to dynamically change the
> sorting.  For the glibc implementation, this can be achieved with the
> `/etc/gai.conf` file.

## File Servers

| File Server | Scenario |
|:-----------:|:---------|
| Network File System (NFS) | Linux-based environment |
| Samba | Mixed environment |
| SSHFS | Secure transfer; only need to access the remote FS occasionally [1] |

[1] Because SSHFS is easy to use and doesn't need external configuration, if you just want to access a remote file server once or occasionally, with SSHFS you don't have to bother to create an configuratio file as you need for NFS and Samba.

## Monitoring: load average

`cat /proc/loadavg` produces output like the following:

```
0.49 0.77 0.94 1/2698 17529
```

- The first three numbers represent the system's load during a period of 1, 5, and 15 minutes respectively.
- The load that's being referred to represents the number of processes that are waiting on, or currently utilizing, the CPU during each timeframe.

> As a general rule of thumb, it's a good idea to record a baseline of your systems when they are under their normal, expected load. Each system on your network will have a designated purpose and each will have a certain load you can reasonably expect your system to face at any one time. If the systems load average dips too far below or climbs higher than the baseline, then you would want to take a look and find out what's going on. If the load reaches a level where there are more processes than you have cores to handle, that's cause for alarm. (_Mastering Linux Network Administration_)

## Monitoring: `htop`, `iotop`, `ncdu`

`htop` is a good alternative to `top`.

`iotop` shows the "top" for I/O.

`ncdu` is a good alternative to `du`.

## IPv6: don't bother unless having a good reason

> you might be tempted to roll-out IPv6 addresses over IPv4 within your network. However, my suggestion is unless you have a very good reason to do so, don't bother. The depletion of IPv4 addresses only affects the public Internet, not your internal network. While you can certainly roll-out IPv6 internally, there's no benefit to doing so. Given that IPv4 has over 4 billion addresses available, you would need quite a few devices in order to justify IPv6. On the other hand, IPv6 is certainly useful (and will eventually be required) for telecoms. It's also useful for those of you who are studying Cisco exams, as understanding of this topic is required. But for the purposes of this book and setting up Linux networks, IPv6 doesn't justify the administration overhead. (_Mastering Linux Network Administration_)

## Static IP Addresses vs Static Leases

You must configure the static IP addresses on the machines that use them. You must be able to find where those machines are.

The static leases are configured on the DHCP servers so it's a central place for you to manage them.

Therefore, static leases may be more convenient for administration.

## TUN/TAP

According to [1], `TUN` and `TAP` are kernel **virtual** network devices:

| Device | Type | Purpose | Layer | Data | Used for |
|:------:|:-----:|:-------:|:-----:|:----:|:-------:|
| TAP | Virtual | Tunneling | Link | Ethernet frames | Bridging |
| TUN | Virtual | Tunneling | Network | IP packets | Routing |

References:
- [1] [Wikipedia: TUN/TAP](https://en.wikipedia.org/wiki/TUN/TAP)
- [2] [Tun/Tap interface tutorial](https://backreference.org/2010/03/26/tuntap-interface-tutorial/)

## Special IP Addresses

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

## `0.0.0.0/0` and `::/0`

In the context of routing tables, a network destination of 0.0.0.0 is used with a network mask of 0 to depict the **default route** as a destination subnet. This destination is expressed as 0.0.0.0/0 in CIDR notation. It matches all addresses in the IPv4 address space and is present on most hosts, directed towards a local router.

In routing tables, 0.0.0.0 can also appear in the gateway column. This indicates that the gateway to reach the corresponding destination subnet is **unspecified**. This generally means that **no intermediate routing hops are necessary because the system is directly connected to the destination**.

`::/0` is the IPv6 version of `0.0.0.0/0` and serves the same purposes.

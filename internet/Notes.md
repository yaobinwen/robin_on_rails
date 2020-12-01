# Notes

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

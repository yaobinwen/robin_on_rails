# Network

## Overview

This folder is all about the network on Linux operating systems.

## HOWTOs

### How to test if a port on a remote system is reachable

- Use `nc`:
  - `nc -zv 127.0.0.1 80`
  - `nc -zv 127.0.0.1 22 80 8080`
  - `nc -zv 127.0.0.1 20-30`
- Use `netstat`:
  - `sudo netstat -ntlp | grep LISTEN`
- Use the good old `telnet`:
  - `telnet host 22`

### How to view the TCP traffic

Use `tcpdump`: `sudo tcpdump -i eth0`

## Misc

- `avahi-browse`: Browse for mDNS/DNS-SD services using the Avahi daemon.
- [`pyroute2`](https://pypi.org/project/pyroute2/): A pure Python netlink library

## References

- [1] [Unix and Linux Administration Handbook (4th Edition)](https://www.amazon.com/UNIX-Linux-System-Administration-Handbook/dp/0131480057): In 2017 they released the 5th edition.
- [2] [8 Steps to Understanding IP Subnetting](https://www.techopedia.com/6/28587/internet/8-steps-to-understanding-ip-subnetting)
- [3] [How do you calculate the prefix, network, subnet, and host numbers?](https://networkengineering.stackexchange.com/questions/7106/how-do-you-calculate-the-prefix-network-subnet-and-host-numbers)
- [4] [An In-Depth Guide to iptables, the Linux Firewall](https://www.booleanworld.com/depth-guide-iptables-linux-firewall/)
- [5] [Issue #28: Read about DNS](https://github.com/yaobinwen/robin_on_rails/issues/28)
- [6] [Issue #29: Read about network basics](https://github.com/yaobinwen/robin_on_rails/issues/29)
- [7] `Bradley Mitchell` wrote [a lot of articles on Lifewire about networking](https://www.lifewire.com/bradley-mitchell-816228).
- [8] **Recommended** [Computer Networking: A Top-Down Approach (7th Edition)](https://www.amazon.com/Computer-Networking-Top-Down-Approach-7th/dp/0133594149)
- [9] **Recommended** [Computer Networks (5th Edition)](https://www.amazon.com/Computer-Networks-5th-Andrew-Tanenbaum/dp/0132126958)
- [10] [An Introduction to Computer Networks by Peter L Dordal](https://intronetworks.cs.luc.edu/)

## Notes

### Gateway

[gateway](https://internetofthingsagenda.techtarget.com/definition/gateway):

> A gateway is a network node that connects two networks using different protocols together. While a bridge is used to join two similar types of networks, a gateway is used to join two dissimilar networks.

### Genmask

[This answer](https://serverfault.com/a/696992/125167) quotes:

> **Genmask**: The netmask for the destination net; 255.255.255.255 for a host destination and 0.0.0.0 for the default route.

Someone else in the same answer also mentioned that:

> It's just odd that the output of route seems to be the only place the actual term "genmask" is used. Everywhere else it's "netmask." E.g. in my system's man files, "netmask" shows up 207 times, but "genmask" is only mentioned in the route man page.

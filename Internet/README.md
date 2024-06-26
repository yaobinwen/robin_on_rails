# Internet

## Overview

This folder is all about the work about the Internet. For the latest well-organized notes, see [_"Notes.md"_](./Notes.md) in this directory.

## RFC Tracking

The [`RFCs.md`](./RFCs.md) tracks my study of the RFCs.

## Emulators

- [mininet](http://mininet.org/): Emulator for rapid prototyping of Software Defined Networks
  - GitHub page: [mininet](https://github.com/mininet/mininet)
- [Containernet](https://containernet.github.io/): Use Docker containers as hosts in Mininet emulations.
  - GitHub page: [Containernet](https://github.com/containernet/containernet)

## HOWTOs

### How to test if a port on a remote system is reachable

- Use `nc`:
  - `nc -zv 127.0.0.1 80`
  - `nc -zv 127.0.0.1 22 80 8080`
  - `nc -zv 127.0.0.1 20-30`
- Use `lsof`:
  - `sudo lsof -nP -iTCP -sTCP:LISTEN`
- Use `ss`:
  - `sudo ss -tunlp | grep <port>` (`ss`: another utility to investigate sockets)
- Use `netstat`:
  - `sudo netstat -ntlp | grep LISTEN`
- Use the good old `telnet`:
  - `telnet host 22`

### How to view the TCP traffic

Use `tcpdump`: `sudo tcpdump -i eth0`

## Misc

- `avahi-browse`: Browse for mDNS/DNS-SD services using the Avahi daemon.
- `curl`: [Everything curl](https://ec.haxx.se/)
- [`pyroute2`](https://pypi.org/project/pyroute2/): A pure Python netlink library

## References

- [1] [Unix and Linux Administration Handbook (4th Edition)](https://www.amazon.com/UNIX-Linux-System-Administration-Handbook/dp/0131480057): In 2017 they released the 5th edition.
- [2] [8 Steps to Understanding IP Subnetting](https://www.techopedia.com/6/28587/internet/8-steps-to-understanding-ip-subnetting)
- [3] [How do you calculate the prefix, network, subnet, and host numbers?](https://networkengineering.stackexchange.com/questions/7106/how-do-you-calculate-the-prefix-network-subnet-and-host-numbers)
- [4] [Issue #28: Read about DNS](https://github.com/yaobinwen/robin_on_rails/issues/28)
- [6] [Issue #29: Read about network basics](https://github.com/yaobinwen/robin_on_rails/issues/29)
- [7] `Bradley Mitchell` wrote [a lot of articles on Lifewire about networking](https://www.lifewire.com/bradley-mitchell-816228).
- [8] **Recommended** [Computer Networking: A Top-Down Approach (7th Edition)](https://www.amazon.com/Computer-Networking-Top-Down-Approach-7th/dp/0133594149)
- [9] **Recommended** [Computer Networks (5th Edition)](https://www.amazon.com/Computer-Networks-5th-Andrew-Tanenbaum/dp/0132126958)
- [10] [An Introduction to Computer Networks by Peter L Dordal](https://intronetworks.cs.luc.edu/)
- [11] [Network Address Translation (NAT)](https://www.practicalnetworking.net/series/nat/nat/)
  - [11.1] [How Network Address Translation Works](https://computer.howstuffworks.com/nat.htm)
- [12] [TCP/IP Guide (free online version)](http://tcpipguide.com/free/index.htm)
- [13] [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)
- [14] [Book: Policy Routing With Linux](http://www.policyrouting.org/PolicyRoutingBook/ONLINE/TOC.html)

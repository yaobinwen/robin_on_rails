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

## Misc

- `avahi-browse`: Browse for mDNS/DNS-SD services using the Avahi daemon.

## References

- [1] [Unix and Linux Administration Handbook (4th Edition)](https://www.amazon.com/UNIX-Linux-System-Administration-Handbook/dp/0131480057): In 2017 they released the 5th edition.
- [2] [8 Steps to Understanding IP Subnetting](https://www.techopedia.com/6/28587/internet/8-steps-to-understanding-ip-subnetting)
- [3] [How do you calculate the prefix, network, subnet, and host numbers?](https://networkengineering.stackexchange.com/questions/7106/how-do-you-calculate-the-prefix-network-subnet-and-host-numbers)
- [4] [An In-Depth Guide to iptables, the Linux Firewall](https://www.booleanworld.com/depth-guide-iptables-linux-firewall/)
- [5] [Issue #28: Read about DNS](https://github.com/yaobinwen/robin_on_rails/issues/28)
- [6] [Issue #29: Read about network basics](https://github.com/yaobinwen/robin_on_rails/issues/29)
- [7] `Bradley Mitchell` wrote [a lot of articles on Lifewire about networking](https://www.lifewire.com/bradley-mitchell-816228).

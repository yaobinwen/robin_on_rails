# Firewall

## Overview

This document has the notes about firewall on Ubuntu.

## iptables

### General

`iptables` has three structures:

- `tables`: Tables allow you to process packets in specific ways. The default is `filter` table. [2]
- `chains`: Chains are attached to tables. "These chains allow you to inspect traffic at various points, such as when they just arrive on the network interface or just before they're handed over to a process." [2]
- `targets`: A target "decides the fate of a packet, such as allowing or rejecting it." I think it's better to call it an "action".

The workflow is as follows:

- A `table` is used to find all the chains and rules to process the packets.
- A packet arrives, or leaves, or is at a specific point.
- The `chain` for that specific point is selected.
- The `rules` in the chain are matched one by one.
- If a `rule` is matched, the workflow jumps to the `target` to perform the action.
- If none of the rules matches, the `default policy` of the chain is used. By default, "all chains have a default policy of allowing packets."

### Tables

On a modern Linux distributions, there are four tables [2]:

| Name | Default | Function |
|-----:|:-------:|:---------|
| filter | Yes | Decide whether a packet should be allowed to reach its destination. |
| mangle | No |  Alter packet headers in various ways. |
| nat | No | Route packets to different hosts on NAT (Network Address Translation) networks by changing the source and destination addresses of packets. It is often used to allow access to services that can't be accessed directly, because they're on a NAT network. |
| raw | No | Work with packets before the kernel starts tracking its state. |
| security | No | Implement policies based on SELinux security contexts. |

### Chains

Every table has some chains; a kind of chain (not an instance of chain) can appear in different tables.

| Name | raw | mangle | nat | filter | Function |
|-----:|:---:|:------:|:---:|:------:|:---------|
| PREROUTING | x | x | x | | For packets when they just arrive on the network interface. |
| INPUT | | x | | x | For packets just before they are given to a local process. |
| OUTPUT | x | x | x | x | For packets just after they are produced by a local process. |
| FORWARD | | x | | x | For packets that are routed to another host through the current host. |
| POSTROUTING | | x | x | | For packets when they are about to leave the network interface. |

See this diagram:

![iptables chains](https://www.booleanworld.com/wp-content/uploads/2017/06/Untitled-Diagram.png)

### Targets

Some commonly used targets are:

| Name | Terminating | Function |
|-----:|:-----------:|:---------|
| ACCEPT | Yes | Accept the packet. |
| DROP | Yes | Drop the packet. |
| REJECT | Yes | Reject the packet. For TCP: "connection reset"; For UDP or ICMP: "destination host unreachable". |
| LOG | No | Log the packet in kernel logs and `iptables` will keep matching it with the rest of the rules. |

### Rule Specification

[1] says the `rule-specification` has this syntax:

```
rule-specification = [matches...] [target]
match = -m matchname [per-match-options]
target = -j targetname [per-target-options]
```

However, a full rule spec can specify the following parts (see the `PARAMETERS` section in `OPTIONS`):

- `-4, --ipv4`: See [1].
- `-6, --ipv6`: See [1].
- `[!] -p, --protocol`: The protocol of the packets.
- `[!] -s, --source`: The IPs which the packets come from.
- `[!] -d, --destination`: The IPs which the packets go to.
- `-m, --match`: The extensions (explained in [3]) that are used to test the specific properties of the packets.
- `-j, --jump`: The target to jump to.
- `-g, --goto`: The chain to go to.
- `[!] -i, --in-interface`: The interface via which the packets are received.
- `[!] -o, --out-interface`: The interface via which the packets are sent.
- `[!] -f, --fragment`: See [1].
- `-c, --set-counters`: See [1].

### Command Line

Delete in the reverse order [2]:

> When you delete a rule that isn't the last rule, the line numbers change, so you might end up deleting the wrong rules! So, if you're deleting a bunch of rules, you should first delete the ones with the highest line numbers.

Since `iptables` looks at the rules one by one, the order of the rules in the chain is important.

### Related Tools

- `iptables-save`: Dump rules from all chains and filters into standard output.
- `iptables-restore`: Restore rules from standard input.

As [1] says, you can first save all the rules into a local file; edit them in a convenient editor; then load them back to `iptables`.

### References

- [1] [iptables(8)](https://manpages.ubuntu.com/manpages/bionic/en/man8/iptables.8.html)
- [2] [An In-Depth Guide to iptables, the Linux Firewall](https://www.booleanworld.com/depth-guide-iptables-linux-firewall/)
- [3] [iptables-extensions(8)](https://manpages.ubuntu.com/manpages/bionic/en/man8/iptables-extensions.8.html)

## Uncomplicated Firewall (UFW)

### Configuration Files

The default policy configuration file is `/etc/default/ufw`.

The other configuration files are in the folder `/etc/ufw`.

The `6` version is for IPv6. For example, `user.rules` is for IPv4 and `user6.rules` is for IPv6.

### Check, Enable, Disable, Reset

- Check status: `sudo ufw status verbose`
- Enable: `sudo ufw enable`
- Disable: `sudo ufw disable`
- Reset: `sudo ufw reset` (This does **not** change default policies if they were modified earlier.)

### Set Up Default Policies

Examples:

```
sudo ufw default deny incoming
sudo ufw default allow outgoing
```

### Allow Other Connections

- SSH:
  - `sudo ufw allow ssh`, or
  - `sudo ufw allow 22` (This also works when SSH uses a different port.)
- HTTP: `sudo ufw allow http`
- HTTPS: `sudo ufw allow https`
- FTP: `sudo ufw allow ftp`

### References

- [1] [How To Set Up a Firewall with UFW on Ubuntu 14.04](https://www.digitalocean.com/community/tutorials/how-to-set-up-a-firewall-with-ufw-on-ubuntu-14-04)

# Firewall

## 1. Overview

This document has the notes about firewalls on Ubuntu.

On Linux, `netfilter` is a packet filtering system in the kernel and `iptables` is a command-line interface to manipulate `netfilter`. Therefore, to some extent, the terms `iptables` and `netfilter` can be used interchangeably. See the following documents for more information:

- [1] [netfilter.org](https://www.netfilter.org/)
  - [1.1] [Linux netfilter Hacking HOWTO](https://www.netfilter.org/documentation/HOWTO/netfilter-hacking-HOWTO.txt)
  - [1.2] [Netfilter's connection tracking system](https://people.netfilter.org/pablo/docs/login.pdf)
- [2] [iptables(8) for Ubuntu 18.04](https://manpages.ubuntu.com/manpages/bionic/en/man8/iptables.8.html)
- [3] [iptables-extensions(8) for Ubuntu 18.04](https://manpages.ubuntu.com/manpages/bionic/en/man8/iptables-extensions.8.html)

`Uncomplicated Firewall` (`ufw`) s a frontend for `iptables` and is "particularly well-suited for host-based firewalls". See the following documents for more information:

- [4] [Uncomplicated Firewall Wiki](https://wiki.ubuntu.com/UncomplicatedFirewall)
- [5] [How To Set Up a Firewall with UFW on Ubuntu 14.04](https://www.digitalocean.com/community/tutorials/how-to-set-up-a-firewall-with-ufw-on-ubuntu-14-04)

Another important concept about firewalls nowadays is **"stateful"**: Basically, a stateful firewall handles the packets according to the firewall rules as well as the states of the packets. For example, if a packet to the port 3913 belongs to an existing connection (hence state `ESTABLISHED`), the packet can be allowed to enter the system even though the firewall rule itself says incoming traffic to the port 3913 should be dropped. See [6] [Is Linux iptables stateful or stateless?](https://janetpanic.com/is-linux-iptables-stateful-or-stateless/) for more information.

**The following articles are good to read whenever you want to quickly refresh the memory.** Because they are good, I saved a local copy in case they are gone someday:

- [7] [An In-Depth Guide to iptables, the Linux Firewall](https://www.booleanworld.com/depth-guide-iptables-linux-firewall/)
- [8] [Linux 2.4 NAT HOWTO](https://www.netfilter.org/documentation/HOWTO/NAT-HOWTO.txt): Although being quite old, it's still a good introductory article about NAT.

## 2. iptables

### 2.1 Concepts

The model of the packet flow and manipulating is very simple:

<table>
  <tr>
    <td>
      <img src="./iptables-chains.png" />
      <p>Figure 1: `iptables` chains</p>
    </td>
  </tr>
</table>

On the highest level, this diagram has three parts:
- 1). On the leftmost side is the network interface.
- 2). On the rightmost side is the local process.
- 3). The part in between is the Linux kernel, or, to be more exact, the `netfilter` subsystem. Figure 1 shows the five `hook points` which calls `chains` to manipulate packets. See below for more details.

Now let's focus on the third part which is the `netfilter` subsystem.

`netfilter` defines five **"hook points"** where processings of the packets occur. They are shown as rounded rectangles in Figure 1. These hook points are pre-defined and can't be added, removed, or customized.

The processing at each hook point is done by a **"chain of rules"**. In other words, each hook point is associated with at least one **chain of rules**. By default, each hook point is associated with a built-in chain, so people call the hook points using the names of the chains they are associated with. Therefore, poeple don't say "the `PREROUTING` hook point". Instead, they directly say "the `PREROUTING` chain, but what they actually refer to is the hook point which has the built-in `PREROUTING` chain associated with.

A **chain of rules** is similar to the design pattern [_Chain of Responsibility_](https://refactoring.guru/design-patterns/chain-of-responsibility). A network packet traverses through the rules inside the chain. If the packet **matches** (see `match` below) the critiera of a rule, the packet is then **handled as specified** (see `target` below); otherwise, the packet moves on to the next rule. (But this is not exactly how the rules are organized. See `tables` below for more details.)

Each rule consists of a **match** and a **target**. A **match** specify the matching criteria of a network packet; a **target** specifies how the packet should be handled if it matches the criteria. "If there are no match criteria, all packets are considered to match. If there is no target specification, nothing is done to the packets (processing proceeds as if the rule did not exist - except that the packet and byte counters are updated)." [5]

A chain has a **policy** which I think is essentially the "default rule" of the chain. In other words, the policy is executed when the packet matches none of the other rules in the current chain.

Inside a chain, the rules are chained one after the other. When a packet enters a chain, it traverses the rules inside the chain one by one. The rules can be divided into different groups according to the types of function they perform. `netfilter` calls these function groups **"tables"**.

Figure 1 shows that the rules are chained in two levels. On the first level is the **table**; on the next level, or, in other words, inside one table, the rules of that functional group are chained one after another. Therefore, using the `PREROUTING` chain as the example, all the rules in the table `raw` are processed first, followed by all the rules in `connection tracking`, followed by all the rules in `mangle`, followed by all the rules in `nat`.

A note on **chains** and **tables**. As [5] says:

> It is common to refer to "the PREROUTING chain of the nat table," which implies that chains belong to tables. However chains and tables are only partially correlated, and neither really "belongs" to the other. Chains represent hook points in the packet flow, and tables represent the types of processing that can occur.

I also find it a bit bizarre that people seem to treat "tables" as the top level concept and "chains" the next level while most of the packet flow diagrams I've seen are chain-oriented. Figure 1 is an example: it breaks tables into different chains rather than the opposite.

In contrast, the `iptables` command line does treat tables as the first-class citizen: chains of rules are broken into tables. If you don't specify the table, the table `filter` is assumed, so don't be surprised when you type `iptables -S` with the hope to see all the rules in all the tables but in fact only the rules in the `filter` table are shown.

### 2.2 Tables

On a modern Linux distributions, there are five tables [2]:

| Name | Default | Function |
|-----:|:-------:|:---------|
| filter | Yes | Decide whether a packet should be allowed to reach its destination. |
| mangle | No |  Alter packet headers in various ways. |
| nat | No | Route packets to different hosts on NAT (Network Address Translation) networks by changing the source and destination addresses of packets. It is often used to allow access to services that can't be accessed directly, because they're on a NAT network. |
| raw | No | Work with packets before the kernel starts tracking its state. |
| security | No | Implement policies based on SELinux security contexts. |

### 2.3 Chains

Every table has some chains; a kind of chain (not an instance of chain) can appear in different tables.

| Name | raw | mangle | nat | filter | Function |
|-----:|:---:|:------:|:---:|:------:|:---------|
| PREROUTING | x | x | x | | For packets when they just arrive on the network interface. |
| INPUT | | x | | x | For packets just before they are given to a local process. |
| OUTPUT | x | x | x | x | For packets just after they are produced by a local process. |
| FORWARD | | x | | x | For packets that are routed to another host through the current host. |
| POSTROUTING | | x | x | | For packets when they are about to leave the network interface. |

### 2.4 Targets

Some commonly used targets are:

| Name | Terminating | Function |
|-----:|:-----------:|:---------|
| ACCEPT | Yes | Accept the packet. |
| DROP | Yes | Drop the packet. |
| REJECT | Yes | Reject the packet. For TCP: "connection reset"; For UDP or ICMP: "destination host unreachable". |

`iptables-extensions(8)` contains many extended targets, such as `LOG` (Log the packet in kernel logs and `iptables` will keep matching it with the rest of the rules.).

### 2.5 Rule Specification (Match + Target)

`iptables(8)` is the core of `iptables` but `iptables-extensions(8)` offers more functionalities, especially the matching critiera and targets. [2] says the `rule-specification` has this syntax:

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

### 2.6 Command Line

Delete in the reverse order [2]:

> When you delete a rule that isn't the last rule, the line numbers change, so you might end up deleting the wrong rules! So, if you're deleting a bunch of rules, you should first delete the ones with the highest line numbers.

Since `iptables` looks at the rules one by one, the order of the rules in the chain is important.

### 2.7 `iptables-save/restore`

- `iptables-save`: Dump rules from all chains and filters into standard output.
- `iptables-restore`: Restore rules from standard input.

As [2] says, you can first save all the rules into a local file; edit them in a convenient editor; then load them back to `iptables`.

`iptables-restore` takes an input file but there seems to be no documentation of the file format. Probably the reason is `iptables` doesn't want the user to manually edit the file. But [this answer](https://unix.stackexchange.com/a/400203/162971) provides some insight:

> Lines starting with `#` are for comments and are not parsed.
> Blank lines are ignored.
> `*` marks the _table_ name.
> `:` marks the _chain_, followed by the default _policy_ and optionally the packet and byte counters.
> byte counters can precede a rule.
> Rules are exactly as given on the command line less the table name.
> Each table section must end with `COMMIT`.
>
> The good news is that the syntax for the actual rules is just as it says in `man iptables`.
>
>     # iptables-restore format
>     *<table>
>     :<chain> <policy> [<packets_count>:<bytes_count>]
>     <optional_counter><rule>
>     ... more rules ...
>     COMMIT
>
>     # iptables-restore example
>     *filter
>     :INPUT DROP [0:0]
>     -A INPUT -s 127.0.0.1 -p tcp -m tcp --dport 9000 -J ACCEPT
>     -A INPUT -p tcp -m tcp --dport 9000 -j REJECT --reject-with icmp-port-unreachable
>     COMMIT

## 3. Uncomplicated Firewall (UFW)

### 3.1 Configuration Files

The default policy configuration file is `/etc/default/ufw`.

The other configuration files are in the folder `/etc/ufw`.

The `6` version is for IPv6. For example, `user.rules` is for IPv4 and `user6.rules` is for IPv6.

### 3.2 Check, Enable, Disable, Reset

- Check status: `sudo ufw status verbose`
- Enable: `sudo ufw enable`
- Disable: `sudo ufw disable`
- Reset: `sudo ufw reset` (This does **not** change default policies if they were modified earlier.)

### 3.3 Set Up Default Policies

Examples:

```
sudo ufw default deny incoming
sudo ufw default allow outgoing
```

### 3.4 Allow Other Connections

- SSH:
  - `sudo ufw allow ssh`, or
  - `sudo ufw allow 22` (This also works when SSH uses a different port.)
- HTTP: `sudo ufw allow http`
- HTTPS: `sudo ufw allow https`
- FTP: `sudo ufw allow ftp`

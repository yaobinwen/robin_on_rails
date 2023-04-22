# Ubuntu DNS Lookup

Study how DNS lookup works on Ubuntu.

[Anatomy of a Linux DNS Lookup: Part I](https://zwischenzugs.com/2018/06/08/anatomy-of-a-linux-dns-lookup-part-i/)

## My notes

`nsswitch.conf` defines how to find the address of a hostname:
- `files`
- `mdns`
- `dns`

[1] says:

> The default `netplan` configuration files in Ubuntu Core leave management of networking devices to networkd. But, when network-manager is installed, it creates new `netplan` configuration files, setting itself as the default network renderer and taking control of all devices.

So:
- `netplan` is similar to the "meta" management system.
- `systemd-networkd` or Network Manager actually manage all the network devices.

## References

- [1] [NetworkManager and netplan](https://ubuntu.com/core/docs/networkmanager/networkmanager-and-netplan)

## TODO

- [ ] What protocol does it use? TCP? UDP? (I remember it's UDP.) Find source.
  - https://en.wikipedia.org/wiki/Domain_Name_System#DNS-over-UDP/53_(%22Do53%22)
  - Need to find the references in the RFCs.
- [ ] Learn what a CNAME record is.
- [ ] [How exactly are NetworkManager, networkd, netplan, ifupdown2, and iproute2 interacting?](https://unix.stackexchange.com/q/475146/162971)

`/etc/resolv.conf` defines the interface query.
- Difference betwwen `resolv.conf` and `stub-resolv.conf`.
- "127.0.0.53": https://unix.stackexchange.com/q/612416/162971
- `systemd`:
  - https://askubuntu.com/q/850339/514711
  - https://unix.stackexchange.com/q/639764/162971
  - (historically) `networking.service`
  - `systemd-networkd.service`
  - `network-mananger.service`
  - `systemd-resolv.service`
- `dnsmasq`: Not installed by default. (Go find the `manifest` file to prove this.)
- `systemd-networkd`, `NetworkManager` and `netplan`:
  - According to https://netplan.io/reference/, `netplan` supports two "network renderers": `systemd-networkd` and `NetworkManager`, so that means `systemd-networkd` and `NetworkManager` play the same role.
- `resolvconf`
- mDNS: Go find the `manifest` file to see if it's installed.
- `/etc/network`
- `dhclient` (isc-dhcp-client)
- `ifup` and `ifdown`
- Other DNS services (e.g., Consul)

## Programs

- `ping`
- `host`
- `dig`


# README

## Overview

This document is intended for VPN as a general topic, but I learned about VPN via the specific product `OpenVPN` so this document has a lot of information about it.

## OpenVPN Setup Consideration

What to take into account when setting up an OpenVPN:

- Hardening the security.
- Verify certificate revocation list.
- Bridging vs routing.
- Routing all client traffic (including web traffic) through the VPN?

## Security Consideration

See [1], [3] and [4].

Key takeaways from [1]:
- It's not recommended to use `tcp` as the connection protocol on OpenVPN because the "double-tcp" structure may cause a lot of issues.
- Do not use `DES` or `3DES`. `DES` is not secure anymore; `3DES` is secure but requires too much CPU computation.
- MD5 is not secure. Use SHA-1.
- Do not trust the so-called "clientless" VPNs. "Public clients are by definition untrusted clients. VPNs work on the foundation that both sides of the connection are trusted." [1] So a VPN client is always needed.

## References

- [1] [OpenVPN and the SSL VPN Revolution](https://www.sans.org/reading-room/whitepapers/vpns/paper/1459)
- [2] [OpenVPN: How To](https://openvpn.net/community-resources/how-to/)
- [3] [OpenVPN: Recommendations to improve security after installation](https://openvpn.net/vpn-server-resources/recommendations-to-improve-security-after-installation/)
- [4] [OpenVPN: Hardening OpenVPN Security](https://openvpn.net/community-resources/hardening-openvpn-security/)

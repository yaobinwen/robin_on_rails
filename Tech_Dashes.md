# Tech Dashes

## Tech Dash: 2022-04-09 ~ 2022-04-10

In this tech dash, I started with the study of [_Atomic Commit In SQLite_](https://www.sqlite.org/atomiccommit.html) which made me wonder how Linux stores/accesses files on the hard drive. As a result, I started to read the chapter about Virtual Filesystem in _Understanding the Linux Kernel_.

While reading _Understanding the Linux Kernel_, I realized I couldn't finish reading the topic because it's a huge one. I have to gradually read the entire book. Therefore, I also read the first chapter _Introduction_.

## Tech Dash: OpenVPN (03/02/2021)

- [x] Fundamental knowledge
  - [x] What is `tun` or `tap` interface?
    - [x] [Wiki: TUN/TAP](https://en.wikipedia.org/wiki/TUN/TAP)
  - [x] How to allow or deny a `tun` or `tap` interface in `ufw`? (`ufw` policy is defined for a specific interface, not for a type of interfaces.)
- [x] How (Open)VPN works?
  - [x] What is the use of the certificates and the keys?
    - [x] [The SSL/TLS Handshake: an Overview](https://www.ssl.com/article/ssl-tls-handshake-overview/)
  - [x] [OpenVPN and the SSL VPNRevolution](https://www.sans.org/reading-room/whitepapers/vpns/openvpn-ssl-vpn-revolution-1459)
  - [x] [What Is OpenVPN & How Does OpenVPN Work?](https://www.cactusvpn.com/beginners-guide-to-vpn/what-is-openvpn/)
- [x] How to revoke properly?
  - [x] [Revoking Certificates](https://openvpn.net/community-resources/revoking-certificates/)
- [x] OpenVPN security:
  - [x] [Hardening OpenVPN Security](https://openvpn.net/community-resources/hardening-openvpn-security/)

## Tech Dash: Security (02/27/2021 ~ 02/28/2021)

In this session, I read about two topics:
- 1). How secure is secure enough?
- 2). What is security testing?

Both are big topics and I could only briefly learn what they are mainly about. I'll need more follow-up sessions to learn deeper.

- [x] How secure is secure enough?
  - [x] https://security.stackexchange.com/a/94830/80050 (This is about general risk assessment, not specifically to the IT industry.)
  - [x] [How Much Security Is Enough? Practitioners Weigh In](https://www.darkreading.com/operations/how-much-security-is-enough-practitioners-weigh-in/d/d-id/1336138) (This article talks about the current security situation in the industry.)
  - [x] [How secure is secure enough?](https://www.newelectronics.co.uk/electronics-technology/how-secure-is-secure-enough/222122/) (This article talks about embedded system security.)
  - [x] [How Secure is Secure Enough?](https://www.computerworld.com/article/2551178/how-secure-is-secure-enough-.html) (Nonsense)
  - [x] [How to communicate how secure your system is to your employer's clients](https://security.stackexchange.com/q/96002/80050)
- [x] Security testing
  - [x] [What is Security Testing? Types with Example](https://www.guru99.com/what-is-security-testing.html)
  - [x] [Security Testing (A Complete Guide)](https://www.softwaretestinghelp.com/how-to-test-application-security-web-and-desktop-application-security-testing-techniques/)

## Tech Dash: Chinese New Year 2021 (02/13/2021 ~ 02/15/2021)

- [x] First of all, something fun: [Particubes](https://docs.particubes.com/)
- [x] Security:
  - [x] [Thinking in Security](https://yaobinwen.github.io/2021/02/14/Thinking-in-security.html)
- [x] Microservices
  - [x] Building Microservices: Chapter 05
- [x] Racket
  - [x] [Why Racket? Why Lisp?](https://beautifulracket.com/appendix/why-racket-why-lisp.html)
  - [x] [Why language-oriented programming? Why Racket?](https://beautifulracket.com/appendix/why-lop-why-racket.html)

## Tech Dash: 2021-01-23 Weekend

During this weekend, I studied three subjects: virtual ethernet; network namespace; Docker networking. I read the following articles:

- [x] [veth(4)](https://man7.org/linux/man-pages/man4/veth.4.html)
- [x] [Introduction to Linux interfaces for virtual networking](https://developers.redhat.com/blog/2018/10/22/introduction-to-linux-interfaces-for-virtual-networking/)
- [x] [network_namespace(7)](https://man7.org/linux/man-pages/man7/network_namespaces.7.html)
- [x] [Introducing Linux Network Namespaces](https://blog.scottlowe.org/2013/09/04/introducing-linux-network-namespaces/)
- [x] https://github.com/moby/libnetwork/blob/master/docs/design.md
- [x] [The Container Networking Landscape: CNI from CoreOS and CNM from Docker](https://thenewstack.io/container-networking-landscape-cni-coreos-cnm-docker/)
- [x] [Understanding Docker Networking Drivers and their use cases](https://www.docker.com/blog/understanding-docker-networking-drivers-use-cases/)

I planned to read more, but I didn't find enough time to finish reading all of them. Besides, I realized I couldn't just read the articles. I must make notes in order to integrate what I've read into my knowledge base. I'll need several follow-up Tech Dash sessions to finish that.

## Tech Dash: Christmas 2020 / New Year 2021

The approaching Christmas 2020 may be a good opportunity to spend some time looking at various technical topics I've been wanting to learn or refresh my memory for a while. The COVID-19 pandemic is still going on anyway, so there is few places (or people) I can visit. I don't plan to look deep into each topic but to learn "just enough for now", because each topic itself may need a whole book to discuss.

Here is the list:

- [x] Python
  - [x] `BrokenPipeError`
  - [x] Python: see what modules are available for use now.
- [x] Security
  - [x] `AppArmor` on Ubuntu
  - [x] [TLS SNI](https://www.cloudflare.com/learning/ssl/what-is-sni/)
  - [x] LDAP
- [x] Network
  - [x] Read: _Mastering Linux Network Administration_.
- [x] Linode: Learn about what `Linode` is.

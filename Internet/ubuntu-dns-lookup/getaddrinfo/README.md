# Study of _What does getaddrinfo do?_

The article [_What does getaddrinfo do?_](https://jameshfisher.com/2018/02/03/what-does-getaddrinfo-do/) is a good one that explains what [`getaddrinfo(3)`](http://manpages.ubuntu.com/manpages/jammy/man3/getaddrinfo.3.html) does. This document contains the notes I make when learning it.

I've written a similar program `demo_getaddrinfo.c` to show how it works. Run `build.sh` to build it. The output program is called `demo_getaddrinfo.exe`.

## Questions:

- [ ] Try to find the code in `gaih_inet()` that correspond to the output below.
  - [ ] Especially, I want to know what function is called to each `libnss_X.so`.
- [ ] Read about [_29 System Databases and Name Service Switch_](https://www.gnu.org/software/libc/manual/html_node/Name-Service-Switch.html).

## Ubuntu Distribution: Focal

```
Distributor ID:	Ubuntu
Description:	Ubuntu 20.04.3 LTS
Release:	20.04
Codename:	focal
```

## `getaddrinfo()` Source Code

https://code.launchpad.net/ubuntu/+source/glibc

`sysdeps/posix/getaddrinfo.c`

But the function `gaih_inet()` seems to do the real work of converting a hostname to an Internet address.

## `demo_getaddrinfo` Output

```
> execve("./demo_getaddress.exe", ["./demo_getaddress.exe", "www.google.com"], 0x7ffda85ac428 /* 24 vars */) = 0
> brk(NULL)                               = 0x564786be3000
> arch_prctl(0x3001 /* ARCH_??? */, 0x7fff4102a080) = -1 EINVAL (Invalid argument)
> access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
> openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=60148, ...}) = 0
> mmap(NULL, 60148, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f12e7e47000
> close(3)                                = 0
> openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
> read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\360q\2\0\0\0\0\0"..., 832) = 832
> pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
> pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
> pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\t\233\222%\274\260\320\31\331\326\10\204\276X>\263"..., 68, 880) = 68
> fstat(3, {st_mode=S_IFREG|0755, st_size=2029224, ...}) = 0
> mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f12e7e45000
> pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
> pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
> pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\t\233\222%\274\260\320\31\331\326\10\204\276X>\263"..., 68, 880) = 68
> mmap(NULL, 2036952, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f12e7c53000
> mprotect(0x7f12e7c78000, 1847296, PROT_NONE) = 0
> mmap(0x7f12e7c78000, 1540096, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x25000) = 0x7f12e7c78000
> mmap(0x7f12e7df0000, 303104, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x19d000) = 0x7f12e7df0000
> mmap(0x7f12e7e3b000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7f12e7e3b000
> mmap(0x7f12e7e41000, 13528, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f12e7e41000
> close(3)                                = 0
> arch_prctl(ARCH_SET_FS, 0x7f12e7e46540) = 0
> mprotect(0x7f12e7e3b000, 12288, PROT_READ) = 0
> mprotect(0x5647866ac000, 4096, PROT_READ) = 0
> mprotect(0x7f12e7e83000, 4096, PROT_READ) = 0
> munmap(0x7f12e7e47000, 60148)           = 0
> socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_ROUTE) = 3
> bind(3, {sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, 12) = 0
> getsockname(3, {sa_family=AF_NETLINK, nl_pid=8027, nl_groups=00000000}, [12]) = 0
> sendto(3, {{len=20, type=RTM_GETADDR, flags=NLM_F_REQUEST|NLM_F_DUMP, seq=1644638379, pid=0}, {ifa_family=AF_UNSPEC, ...}}, 20, 0, {sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, 12) = 20
> recvmsg(3, {msg_name={sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, msg_namelen=12, msg_iov=[{iov_base=[{{len=76, type=RTM_NEWADDR, flags=NLM_F_MULTI, seq=1644638379, pid=8027}, {ifa_family=AF_INET, ifa_prefixlen=8, ifa_flags=IFA_F_PERMANENT, ifa_scope=RT_SCOPE_HOST, ifa_index=if_nametoindex("lo")}, [{{nla_len=8, nla_type=IFA_ADDRESS}, inet_addr("127.0.0.1")}, {{nla_len=8, nla_type=IFA_LOCAL}, inet_addr("127.0.0.1")}, {{nla_len=7, nla_type=IFA_LABEL}, "lo"}, {{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_PERMANENT}, {{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=4294967295, ifa_valid=4294967295, cstamp=164, tstamp=164}}]}, {{len=88, type=RTM_NEWADDR, flags=NLM_F_MULTI, seq=1644638379, pid=8027}, {ifa_family=AF_INET, ifa_prefixlen=24, ifa_flags=0, ifa_scope=RT_SCOPE_UNIVERSE, ifa_index=if_nametoindex("enp0s3")}, [{{nla_len=8, nla_type=IFA_ADDRESS}, inet_addr("192.168.50.6")}, {{nla_len=8, nla_type=IFA_LOCAL}, inet_addr("192.168.50.6")}, {{nla_len=8, nla_type=IFA_BROADCAST}, inet_addr("192.168.50.255")}, {{nla_len=11, nla_type=IFA_LABEL}, "enp0s3"}, {{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_NOPREFIXROUTE}, {{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=85904, ifa_valid=85904, cstamp=1289, tstamp=1310}}]}], iov_len=4096}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, 0) = 164
> brk(NULL)                               = 0x564786be3000
> brk(0x564786c04000)                     = 0x564786c04000
> recvmsg(3, {msg_name={sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, msg_namelen=12, msg_iov=[{iov_base=[{{len=72, type=RTM_NEWADDR, flags=NLM_F_MULTI, seq=1644638379, pid=8027}, {ifa_family=AF_INET6, ifa_prefixlen=128, ifa_flags=IFA_F_PERMANENT, ifa_scope=RT_SCOPE_HOST, ifa_index=if_nametoindex("lo")}, [{{nla_len=20, nla_type=IFA_ADDRESS}, inet_pton(AF_INET6, "::1")}, {{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=4294967295, ifa_valid=4294967295, cstamp=164, tstamp=164}}, {{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_PERMANENT}]}, {{len=72, type=RTM_NEWADDR, flags=NLM_F_MULTI, seq=1644638379, pid=8027}, {ifa_family=AF_INET6, ifa_prefixlen=64, ifa_flags=IFA_F_SECONDARY, ifa_scope=RT_SCOPE_UNIVERSE, ifa_index=if_nametoindex("enp0s3")}, [{{nla_len=20, nla_type=IFA_ADDRESS}, inet_pton(AF_INET6, "2601:547:900:156e:8e2e:e1b3:fd59:2121")}, {{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=599, ifa_valid=599, cstamp=858, tstamp=50309}}, {{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_SECONDARY}]}, {{len=72, type=RTM_NEWADDR, flags=NLM_F_MULTI, seq=1644638379, pid=8027}, {ifa_family=AF_INET6, ifa_prefixlen=64, ifa_flags=0, ifa_scope=RT_SCOPE_UNIVERSE, ifa_index=if_nametoindex("enp0s3")}, [{{nla_len=20, nla_type=IFA_ADDRESS}, inet_pton(AF_INET6, "2601:547:900:156e:ebb9:def8:f7d2:3a45")}, {{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=599, ifa_valid=599, cstamp=858, tstamp=50309}}, {{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_MANAGETEMPADDR|IFA_F_NOPREFIXROUTE}]}, {{len=72, type=RTM_NEWADDR, flags=NLM_F_MULTI, seq=1644638379, pid=8027}, {ifa_family=AF_INET6, ifa_prefixlen=64, ifa_flags=IFA_F_PERMANENT, ifa_scope=RT_SCOPE_LINK, ifa_index=if_nametoindex("enp0s3")}, [{{nla_len=20, nla_type=IFA_ADDRESS}, inet_pton(AF_INET6, "fe80::9aa0:774e:7ae1:36e")}, {{nla_len=20, nla_type=IFA_CACHEINFO}, {ifa_prefered=4294967295, ifa_valid=4294967295, cstamp=524, tstamp=50309}}, {{nla_len=8, nla_type=IFA_FLAGS}, IFA_F_PERMANENT|IFA_F_NOPREFIXROUTE}]}], iov_len=4096}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, 0) = 288
> recvmsg(3, {msg_name={sa_family=AF_NETLINK, nl_pid=0, nl_groups=00000000}, msg_namelen=12, msg_iov=[{iov_base={{len=20, type=NLMSG_DONE, flags=NLM_F_MULTI, seq=1644638379, pid=8027}, 0}, iov_len=4096}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, 0) = 20
> socket(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0) = 4
> connect(4, {sa_family=AF_UNIX, sun_path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)
> close(4)                                = 0
> close(3)                                = 0
> socket(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0) = 3
> connect(3, {sa_family=AF_UNIX, sun_path="/var/run/nscd/socket"}, 110) = -1 ENOENT (No such file or directory)
> close(3)                                = 0
```

```
> openat(AT_FDCWD, "/etc/nsswitch.conf", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=542, ...}) = 0
> read(3, "# /etc/nsswitch.conf\n#\n# Example"..., 4096) = 542
> read(3, "", 4096)                       = 0
> close(3)                                = 0
```

Read `/etc/host.conf`:

```
> stat("/etc/resolv.conf", {st_mode=S_IFREG|0644, st_size=728, ...}) = 0
> openat(AT_FDCWD, "/etc/host.conf", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=92, ...}) = 0
> read(3, "# The \"order\" line is only used "..., 4096) = 92
> read(3, "", 4096)                       = 0
> close(3)                                = 0
```

Read `/etc/resolv.conf`:

```
> openat(AT_FDCWD, "/etc/resolv.conf", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=728, ...}) = 0
> read(3, "# This file is managed by man:sy"..., 4096) = 728
> read(3, "", 4096)                       = 0
> close(3)                                = 0
```

Read `/etc/ld.so.cache` (because it needs to load the `nss` dynamic libraries):

```
> openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=60148, ...}) = 0
> mmap(NULL, 60148, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f12e7e47000
> close(3)                                = 0
```

Read `/lib/x86_64-linux-gnu/libnss_files.so.2` because `nsswitch.conf` specifies `files`:
- `libnss_files.so.2` is part of the package `libc6`.
- On Ubuntu 20.04.3, `libc6` is at the version `2.31-0ubuntu9.2`.
- The source code is [here](https://code.launchpad.net/ubuntu/+source/glibc).

```
> openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libnss_files.so.2", O_RDONLY|O_CLOEXEC) = 3
> read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\3005\0\0\0\0\0\0"..., 832) = 832
> fstat(3, {st_mode=S_IFREG|0644, st_size=51832, ...}) = 0
> mmap(NULL, 79672, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f12e7c3f000
> mmap(0x7f12e7c42000, 28672, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7f12e7c42000
> mmap(0x7f12e7c49000, 8192, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xa000) = 0x7f12e7c49000
> mmap(0x7f12e7c4b000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xb000) = 0x7f12e7c4b000
> mmap(0x7f12e7c4d000, 22328, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f12e7c4d000
> close(3)                                = 0
```

Actually read the `/etc/hosts` file:

```
> mprotect(0x7f12e7c4b000, 4096, PROT_READ) = 0
> munmap(0x7f12e7e47000, 60148)           = 0
> openat(AT_FDCWD, "/etc/hosts", O_RDONLY|O_CLOEXEC) = 3
> lseek(3, 0, SEEK_CUR)                   = 0
> fstat(3, {st_mode=S_IFREG|0644, st_size=241, ...}) = 0
> read(3, "127.0.0.1\tlocalhost\n127.0.1.1\tyw"..., 4096) = 241
> lseek(3, 0, SEEK_CUR)                   = 241
> read(3, "", 4096)                       = 0
> close(3)                                = 0
```

Read the dynamic library cache again because the program needs to load `libnss_mdns4_minimal.so.2`:

```
> openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=60148, ...}) = 0
> mmap(NULL, 60148, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f12e7e47000
> close(3)                                = 0
```

Load `libnss_mdns4_minimal.so.2`:
- This library is installed by the package `libnss-mdns` which is (taken from its `apt-cache show` output):

```
Description-en: NSS module for Multicast DNS name resolution
 nss-mdns is a plugin for the GNU Name Service Switch (NSS) functionality
 of the GNU C Library (glibc) providing host name resolution via Multicast
 DNS (using Zeroconf, aka Apple Bonjour / Apple Rendezvous ), effectively
 allowing name resolution by common Unix/Linux programs in the ad-hoc mDNS
 domain .local.
```

```
> openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libnss_mdns4_minimal.so.2", O_RDONLY|O_CLOEXEC) = 3
> read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\240\23\0\0\0\0\0\0"..., 832) = 832
> fstat(3, {st_mode=S_IFREG|0644, st_size=18504, ...}) = 0
> mmap(NULL, 20496, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f12e7c39000
> mmap(0x7f12e7c3a000, 8192, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1000) = 0x7f12e7c3a000
> mmap(0x7f12e7c3c000, 4096, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7f12e7c3c000
> mmap(0x7f12e7c3d000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7f12e7c3d000
> close(3)                                = 0
```

What does this block do??

```
> openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libresolv.so.2", O_RDONLY|O_CLOEXEC) = 3
> read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0 G\0\0\0\0\0\0"..., 832) = 832
> fstat(3, {st_mode=S_IFREG|0644, st_size=101320, ...}) = 0
> mmap(NULL, 113280, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f12e7c1d000
> mprotect(0x7f12e7c21000, 81920, PROT_NONE) = 0
> mmap(0x7f12e7c21000, 65536, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x4000) = 0x7f12e7c21000
> mmap(0x7f12e7c31000, 12288, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x14000) = 0x7f12e7c31000
> mmap(0x7f12e7c35000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x17000) = 0x7f12e7c35000
> mmap(0x7f12e7c37000, 6784, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f12e7c37000
> close(3)                                = 0
```

```
> mprotect(0x7f12e7c35000, 4096, PROT_READ) = 0
> mprotect(0x7f12e7c3d000, 4096, PROT_READ) = 0
> munmap(0x7f12e7e47000, 60148)           = 0
```

Read `ld.so.cache` because the program needs to load `libnss_dns.so.2`:

```
> openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=60148, ...}) = 0
> mmap(NULL, 60148, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f12e7e47000
> close(3)                                = 0
```

Load `libnss_dns.so.2`:

```
> openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libnss_dns.so.2", O_RDONLY|O_CLOEXEC) = 3
> read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0 #\0\0\0\0\0\0"..., 832) = 832
> fstat(3, {st_mode=S_IFREG|0644, st_size=31176, ...}) = 0
> mmap(NULL, 32984, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f12e7c14000
> mmap(0x7f12e7c16000, 16384, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7f12e7c16000
> mmap(0x7f12e7c1a000, 4096, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x6000) = 0x7f12e7c1a000
> mmap(0x7f12e7c1b000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x6000) = 0x7f12e7c1b000
> close(3)                                = 0
```

Perform DNS query:

```
> mprotect(0x7f12e7c1b000, 4096, PROT_READ) = 0
> munmap(0x7f12e7e47000, 60148)           = 0
> socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, IPPROTO_IP) = 3
> setsockopt(3, SOL_IP, IP_RECVERR, [1], 4) = 0
> connect(3, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("127.0.0.53")}, 16) = 0
> poll([{fd=3, events=POLLOUT}], 1, 0)    = 1 ([{fd=3, revents=POLLOUT}])
> sendmmsg(3, [{msg_hdr={msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="2\335\1 \0\1\0\0\0\0\0\1\3www\6google\3com\0\0\1\0\1\0"..., iov_len=42}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, msg_len=42}, {msg_hdr={msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="i\313\1 \0\1\0\0\0\0\0\1\3www\6google\3com\0\0\34\0\1\0"..., iov_len=42}], msg_iovlen=1, msg_controllen=0, msg_flags=0}, msg_len=42}], 2, MSG_NOSIGNAL) = 2
> poll([{fd=3, events=POLLIN}], 1, 5000)  = 1 ([{fd=3, revents=POLLIN}])
> ioctl(3, FIONREAD, [127])               = 0
> recvfrom(3, "2\335\201\200\0\1\0\4\0\0\0\1\3www\6google\3com\0\0\1\0\1\300"..., 2048, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("127.0.0.53")}, [28->16]) = 127
> poll([{fd=3, events=POLLIN}], 1, 4997)  = 1 ([{fd=3, revents=POLLIN}])
> ioctl(3, FIONREAD, [95])                = 0
> recvfrom(3, "i\313\201\200\0\1\0\2\0\0\0\1\3www\6google\3com\0\0\34\0\1\300"..., 65536, 0, {sa_family=AF_INET, sin_port=htons(53), sin_addr=inet_addr("127.0.0.53")}, [28->16]) = 95
> close(3)                                = 0
```

```
> openat(AT_FDCWD, "/etc/gai.conf", O_RDONLY|O_CLOEXEC) = 3
> fstat(3, {st_mode=S_IFREG|0644, st_size=2584, ...}) = 0
> fstat(3, {st_mode=S_IFREG|0644, st_size=2584, ...}) = 0
> read(3, "# Configuration for getaddrinfo("..., 4096) = 2584
> read(3, "", 4096)                       = 0
> close(3)                                = 0
```

Verify that the IP address is correct:

```
> socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, IPPROTO_IP) = 3
> connect(3, {sa_family=AF_INET, sin_port=htons(0), sin_addr=inet_addr("142.250.31.104")}, 16) = 0
> getsockname(3, {sa_family=AF_INET, sin_port=htons(36562), sin_addr=inet_addr("192.168.50.6")}, [28->16]) = 0
> connect(3, {sa_family=AF_UNSPEC, sa_data="\0\0\0\0\0\0\0\0\0\0\0\0\0\0"}, 16) = 0
> connect(3, {sa_family=AF_INET, sin_port=htons(0), sin_addr=inet_addr("104.193.88.123")}, 16) = 0
> getsockname(3, {sa_family=AF_INET, sin_port=htons(47997), sin_addr=inet_addr("192.168.50.6")}, [28->16]) = 0
> close(3)                                = 0
```

Print the result:

```
> fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
> write(1, "'www.google.com' is at '104.193.8"..., 38'www.google.com' is at '142.250.31.104'
> ) = 38
> exit_group(0)                           = ?
> +++ exited with 0 +++
```

## `/etc/nsswitch.conf`

```
# /etc/nsswitch.conf
#
# Example configuration of GNU Name Service Switch functionality.
# If you have the `glibc-doc-reference' and `info' packages installed, try:
# `info libc "Name Service Switch"' for information about this file.

passwd:         files systemd
group:          files systemd
shadow:         files
gshadow:        files

hosts:          files mdns4_minimal [NOTFOUND=return] dns
networks:       files

protocols:      db files
services:       db files
ethers:         db files
rpc:            db files

netgroup:       nis
```

## `/etc/host.conf`

```
# The "order" line is only used by old versions of the C library.
order hosts,bind
multi on
```

According to [_The /etc/host.conf File_](http://www.linfo.org/etc_host_conf.html):

>  The order `option` specifies the order in which the resolving services are tried. Valid choices are `bind` for querying the _BIND_ name server, `hosts` for lookups in the `/etc/hosts` file, and `nis` for NIS (network information service) lookups. Any or all of them may be specified. The order in which they appear on the line determines the order in which the respective services are tried by the _resolver library_. The above example specifies that `/etc/hosts` should be consulted first, and then _BIND_ if necessary.
>
> The `multi` option determines whether a host in the `/etc/hosts` file can have multiple IP addresses. It takes `on` or `off` as options. The default is `off`, as it may cause a substantial performance loss at sites with large hosts files.. This option has no effect on DNS or NIS queries.

## `/etc/resolv.conf`

`/etc/resolv.conf` is a symbolic link to `/run/systemd/resolve/stub-resolv.conf`:

```
# This file is managed by man:systemd-resolved(8). Do not edit.
#
# This is a dynamic resolv.conf file for connecting local clients to the
# internal DNS stub resolver of systemd-resolved. This file lists all
# configured search domains.
#
# Run "resolvectl status" to see details about the uplink DNS servers
# currently in use.
#
# Third party programs must not access this file directly, but only through the
# symlink at /etc/resolv.conf. To manage man:resolv.conf(5) in a different way,
# replace this symlink by a static file or a different symlink.
#
# See man:systemd-resolved.service(8) for details about the supported modes of
# operation for /etc/resolv.conf.

nameserver 127.0.0.53
options edns0 trust-ad
search lan
```

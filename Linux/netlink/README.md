# Netlink Socket

## 1. Overview

As [1] puts, `netlink` is implemented for "communication between kernel and user space". [1] is condensed so you may still need to read other articles to fully understand a topic, but it covers pretty much everything about `netlink`.

[3] is a good introductory article to get an overview about netlink socket. Unfortunately, its example source code doesn't compile.

## 2. What Problem Does `netlink` Solve?

Linux kernel only contains the "most essential and performance-critical code". Everything else is typically implemented as user-space applications. Therefore, a question arises naturally: **How can kernel code and user-space code communicate?** The answer is the various inter-process communication (IPC) methods that exist between kernel and user space:
- system call
- `ioctl`
- `proc` filesystem
- **`netlink` socket**

## 3. Characteristics

| Advantage | Description |
|:---------:|:------------|
| Async | The processing time is minimized so the kernel's scheduling won't be affected too much. |
| Multicast | So multiple receivers can be notified at the same time. No need multiple notifications or inquiries. |
| Full-duplex | System calls and `ioctl` can only be initiated by the user-space code; `netlink` can also be initiated by the kernel.|
| Easy to learn | `netlink` uses BSD-like socket APIs so the community is already familiar with them. |

## 4. Programming

### 4.1 Overview

`netlink` consists of "a **standard sockets-based interface** for user space processes and an internal kernel API for kernel modules" [1]. It uses the following API functions:
- `socket`
- `bind`
- `sendmsg`
- `recvmsg`
- `close`
- A set of auxiliary macros to process the `netlink` messages (see [2]).

### 4.2 `socket`

A `netlink` socket is created as follows:

```c
int sock_fd = socket(AF_NETLINK, socket_type, netlink_family);
```

where:

- `netlink` itself is identified by the address family `AF_NETLINK` (see `socket.h`):

```c
/* Protocol families.  */
#define PF_NETLINK  16
#define PF_ROUTE  PF_NETLINK /* Alias to emulate 4.4BSD.  */

/* Address families.  */
#define AF_NETLINK  PF_NETLINK
#define AF_ROUTE  PF_ROUTE
```

- `netlink` is **datagram-oriented**, similar to UDP, so `socket_type` should be either `SOCK_RAW` or `SOCK_DGRAM`.
- `netlink_family` "selects the **kernel module** or **netlink group** to communicate with". An important family is `NETLINK_ROUTE`.

### 4.3 Addresses (Unicast and Multicast)

Two parties are involved in `netlink` communication: user-space `netlink` sockets and the kernel `netlink` socket.

In order for the messages to be delivered to the correct socket, every `netlink` socket must have a unique address to identify itself. This is its **unicast address**.

If one user-space process only uses one `netlink` socket, the socket can use the process's process ID as its address. If one user-space process uses mutliple `netlink` sockets, each socket must figure out a unique address for themselves (and this is usually up to the programmers).

The kernel socket is always identified by the unicast address `0`.

A `netlink` socket can (but doesn't have to) be assigned to one or more **multicast groups**. When a message is sent to a multicast group, all the `netlink` sockets that listen to that multicast group will receive the message. Therefore, a `netlink` socket may have one or more **multicast addresses**.

A `netlink` socket address is defined by `sockaddr_nl` (see `/usr/include/linux/netlink.h`):

```c
struct sockaddr_nl {
  __kernel_sa_family_t  nl_family;  /* AF_NETLINK */
  unsigned short        nl_pad;     /* zero */
  __u32                 nl_pid;     /* port ID */
  __u32                 nl_groups;  /* multicast groups mask */
};
```

The `nl_pid` defines the **unicast address**; the `nl_groups` is "a bit mask with every bit representing a netlink group number" [1].

When a message is sent, the destination address can be identified with the following possible combinations:

| `nl_pid` | `nl_groups` | Destination |
|:--------:|:-----------:|:------------|
| 0 | 0 | kernel |
| N | 0 | user-space process (only unicast) |
| 0 | M | user-space processes (that belong to the multicast groups) |
| N | M | ?? (Not sure if this is valid) |

### 4.4 Message Structure

`netlink` has messages of the general form, but each sub-family protocol, such as `NETLINK_ROUTE`, may have its own extended format of messages. This section only describes the general form.

In the general form, a `netlink` message is described using `struct msghdr`:

```c
struct msghdr
  {
    void *msg_name;         /* Address to send to/receive from.  */
    socklen_t msg_namelen;  /* Length of address data.  */

    struct iovec *msg_iov;  /* Vector of data to send/receive into.  */
    size_t msg_iovlen;      /* Number of elements in the vector.  */

    void *msg_control;      /* Ancillary data (eg BSD filedesc passing). */
    size_t msg_controllen;  /* Ancillary data buffer length.
                               !! The type should be socklen_t but the
                               definition of the kernel is incompatible
                               with this.  */

    int msg_flags;          /* Flags on received message.  */
  };
```

which uses `struct iovec` to list the data it sends/receives:

```c
struct iovec
  {
    void *iov_base;  /* Pointer to data.  */
    size_t iov_len;  /* Length of data.  */
  };
```

`iovec.iov_base` points to the raw byte stream which, in the case of `netlink` messages, can be a list of `struct nlmsghdr`:

```c
struct nlmsghdr {
  __u32    nlmsg_len;     /* Length of message including header */
  __u16    nlmsg_type;    /* Message content */
  __u16    nlmsg_flags;   /* Additional flags */
  __u32    nlmsg_seq;     /* Sequence number */
  __u32    nlmsg_pid;     /* Sending process port ID */
};
```

[4] has an illustration of the message format. I will describe it in my own words as follows.

This raw byte stream is similar to a data structure of `unsigned char * data` which is equivalent to `void * iov_base` and the length of which is `iov_len`. Note that this can contain more than one message, hence multiple occurrences of `struct nlmsghdr`:
- `data[0]` is the first byte of `struct nlmsghdr`; `data[sizeof(struct nlmsghdr)-1]` is the last byte of `struct nlmsghdr`.
- `nlmsg_len` points to the total length of the first message. Suppose its `N` in this example. Therefore, `data[N-1]` is the last byte of this whole message.
- `data[sizeof(struct nlmsghdr)]` up to `data[N-1]` are the bytes of the data for the first message.
  - According to [4], there may be some padding bytes for byte alignment between the header and the first byte of the real data. This is why [1] says "The byte stream should be accessed only with the standard `NLMSG_*` macros." See [2] for further information on those macros.
- If `iov_len` is greater than `N`, that means there is the next message. The starting byte of the next message is `data[N]`. The length of the second message is still described by `nlmsg_len`. Suppose its `M` for the second message, then `data[N+M-1]` is the last byte of the second message.
- `data[N+sizeof(struct nlmsghdr)]` up to `data[N+M-1]` are the bytes of the data for the second message.
- If `iov_len` is greater than `N+M`, that means there is the next message. This process continues until all the messages are examined.

### 4.5 Message Types

`nlmsghdr.nlmsg_type` specifies the type of the message [1]:
- `NLMSG_NOOP`: This message should be ignored.
- `NLMSG_ERROR`: This message signals an error and the payload contains an `struct nlmsgerr` structure.
- `NLMSG_DONE`: This message terminates a multipart message.
  - In multipart messages, the first and all following headers have the `NLM_F_MULTI` flag set, except for the last header which has the type `NLMSG_DONE`.

### 4.6 Programming Libraries

It's often not recommended to program in the low-level kernel APIs. Instead, `libnetlink` [7] and `libnl` [8] are recommended.

`libmnl` [9] is a" minimalistic user-space library oriented to Netlink developers."

## 5. Demo

The code under the `demo` directory demonstrates how to send and receive a message using `netlink` between two user-space processes `send` and `recv`.

Follow the steps below to run the code:
- `mkdir build && cd build`
- `cmake .. && cmake --build .`
- Start `recv` (which is a blocking process): `./recv 20211107`
- Run `send`: `sudo ./send 20211107 "Welcome to Pennsylvania!"`
  - You will need `sudo` to run `send` because otherwise you'll get the error of "Operation not permitted".

## 6. References

- [1] [netlink(7)](http://manpages.ubuntu.com/manpages/bionic/man7/netlink.7.html)
- [2] [netlink(3)](http://manpages.ubuntu.com/manpages/bionic/man3/netlink.3.html)
- [3] [Kernel Korner - Why and How to Use Netlink Socket](https://www.linuxjournal.com/article/7356)
- [4] [Understanding And Programming With Netlink Sockets]()
- [5] [Communicating between the kernel and user-space in Linux using Netlink sockets](https://perso.ens-lyon.fr/laurent.lefevre/pdf/JS2010_Neira_Gasca_Lefevre.pdf)
  - Also see [the copy in this folder](./Communicating-between-the-kernel-and-user-space-in-Linux-using-Netlink-sockets.pdf)
- [6] [Communicating between the kernel and user-space in Linux using Netlink sockets: Source code reference](https://people.netfilter.org/pablo/netlink/netlink-libmnl-manual.pdf)
  - Also see [the copy in this folder](./Communicating-between-the-kernel-and-user-space-in-Linux-using-Netlink-sockets_Source-code-reference.pdf)
- [7] [libnetlink(3): A library for accessing the netlink service](http://manpages.ubuntu.com/manpages/bionic/man3/libnetlink.3.html)
- [8] [libnl: Netlink Protocol Library Suite](http://www.infradead.org/~tgr/libnl/)
- [9] [libmnl: a minimalistic user-space library oriented to Netlink developers](https://www.netfilter.org/projects/libmnl/index.html)
- [10] [generic_netlink_howto](https://wiki.linuxfoundation.org/networking/generic_netlink_howto)

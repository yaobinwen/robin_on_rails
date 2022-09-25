# Kernel

## Hard and Soft Links

Why is it not allowed to create hard links for directories?

> It is not possible to create hard links for directories. Doing so might transform the directory tree into a **graph with cycles**, thus making it impossible to locate a file according to its name.

## Reentrant Kernels

> All Unix kernels are **reentrant**. This means that several processes may be executing in Kernel Mode at the same time.
>
> But a reentrant kernel is not limited only to such reentrant functions (although that is how some real-time kernels are implemented). Instead, the kernel **can include non-reentrant functions and use locking mechanisms to ensure that only one process can execute a non-reentrant function at a time.**

Regarding the last sentence about "non-reentrant functions": Although these functions can change the global state of the system, as long as they are well synchronized using locking mechanisms, the global state can always remain well-defined.

> If a hardware interrupt occurs, a reentrant kernel is able to **suspend the current running process even if that process is in Kernel Mode**. This capability is very important, because it improves the throughput of the device controllers that issue interrupts. Once a device has issued an interrupt, it waits until the CPU acknowledges it. **If the kernel is able to answer quickly, the device controller will be able to perform other tasks while the CPU handles the interrupt.**

## Virtual Filesystem

Virtual Filesystem (also known as _Virtual Filesystem Switch_ or _VFS_).

The _common file model_ is capable of representing all supported file systems:

> This model strictly mirrors the file model provided by the traditional Unix filesystem. This is not surprising, because Linux wants to run its native filesystem with minimum overhead. However, each specific filesystem implementation must translate its physical organization into the VFS's common file model.

## Namespaces

- [1] [Namespaces in operation, part 1: namespaces overview](https://lwn.net/Articles/531114/)
- [2] [Understanding user namespaces](https://man7.org/conf/meetup/understanding-user-namespaces--Google-Munich-Kerrisk-2019-10-25.pdf)

>  The purpose of each namespace is to wrap a particular global system resource in an abstraction that makes it appear to the processes within the namespace that they have their own isolated instance of the global resource. One of the overall goals of namespaces is to support the implementation of containers, a tool for lightweight virtualization (as well as other purposes) that provides a group of processes with the illusion that they are the only processes on the system.

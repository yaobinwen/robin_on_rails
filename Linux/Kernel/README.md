# Kernel

## System calls

The 64-bit system call numbers and entry vectors can be found in the file [linux/arch/x86/entry/syscalls/syscall_64.tbl](https://github.com/torvalds/linux/blob/master/arch/x86/entry/syscalls/syscall_64.tbl).

To find the implementation of a system call, grep for `SYSCALL_DEFINE.\?(<syscall-name>,`. For example, to find the source code of the system call `open`:

```
$ grep -r -A3 "SYSCALL_DEFINE.\?(open,"
--
fs/open.c:SYSCALL_DEFINE3(open, const char __user *, filename, int, flags, umode_t, mode)
fs/open.c-{
fs/open.c-	if (force_o_largefile())
fs/open.c-		flags |= O_LARGEFILE;
--
fs/open.c:COMPAT_SYSCALL_DEFINE3(open, const char __user *, filename, int, flags, umode_t, mode)
fs/open.c-{
fs/open.c-	return do_sys_open(AT_FDCWD, filename, flags, mode);
fs/open.c-}
```

[3.1] explains the meaning of `SYSCALL_DEFINE`:

> ... with the appropriate `SYSCALL_DEFINEn()` macro rather than explicitly. The `n` indicates the number of arguments to the system call, and the macro takes the system call name followed by the (type, name) pairs for the parameters as arguments.

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

## References

- [1] Technical articles online:
  - [1.1] [Marco Cetica: Kernel Hacking - System Calls(3/3)](https://marcocetica.com/posts/kernel_hacking_part3/)
- [2] [Bootlin: Elixir Cross Referencer](https://elixir.bootlin.com/linux/latest/source): A good source cross referencing tool for not only the Linux kernel but also others such as `glibc`. When searching a keyword, it organizes the results as different groups such as "defined in as a member", "defined in as a function", "referenced in", etc. to make it easier to find the wanted matches.
- [3] [The Linux Kernel documentation](https://www.kernel.org/doc/html/latest/index.html)
  - [3.1] [Adding a New System Call](https://www.kernel.org/doc/html/latest/process/adding-syscalls.html)

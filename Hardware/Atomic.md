# Atomic Operations on the Hardware Level

Refer to the question [_How are atomic operations implemented at a hardware level?_](https://stackoverflow.com/q/14758088/630364).

A comment says the _Volume 3A: System Programming Guide, Part 1_, chapter 8 _MULTIPLE-PROCESSOR MANAGEMENT_ describes locked atomic operations.

[This answer](https://stackoverflow.com/a/14761050/630364) mentions an article [_Implementing Scalable Atomic Locks for Multi-Core Intel® EM64T and IA32 Architectures_](https://software.intel.com/en-us/articles/implementing-scalable-atomic-locks-for-multi-core-intel-em64t-and-ia32-architectures) by Intel that doesn't seem to exist anymore, nor could I find it on the [Internet Archive](https://archive.org/).

However, a portion of this mentioned article is quoted in [this Quora answer](https://qr.ae/pvsXUS):

> **User Level Atomic Locks**
>
> User level locks involve utilizing the atomic instructions of processor to atomically update a memory space. The atomic instructions involve utilizing a lock prefix on the instruction and having the destination operand assigned to a memory address. The following instructions can run atomically with a lock prefix on current Intel processors: ADD, ADC, AND, BTC, BTR, BTS, CMPXCHG, CMPXCH8B, DEC, INC, NEG, NOT, OR, SBB, SUB, XOR, XADD, and XCHG. EnterCriticalSection utilizes atomic instructions to attempt to get a user-land lock before jumping into the kernel. On most instructions a lock prefix must be explicitly used except for the xchg instruction where the lock prefix is implied if the instruction involves a memory address.
>
> In the days of Intel 486 processors, the lock prefix used to assert a lock on the bus along with a large hit in performance. Starting with the Intel Pentium Pro architecture, the bus lock is transformed into a cache lock. A lock will still be asserted on the bus in the most modern architectures if the lock resides in uncacheable memory or if the lock extends beyond a cache line boundary splitting cache lines. Both of these scenarios are unlikely, so most lock prefixes will be transformed into a cache lock which is much less expensive.
>
> Figure 3 contains a simple lock written in a few lines of assembly to demonstrate usage of utilizing an atomic instruction with a lock prefix for obtaining a lock. In this example, the code is simply testing a memory space pointed to attempt to get a lock. If the memory space contains a 1, it means another thread has already obtained the lock. If the memory space is 0, it means the lock is free. The atomic xchg instruction is used to attempt to do an exchange of 1 with the memory space. If eax contains 0 after the xchg instruction, it means that the lock was achieved by the current thread. If the eax contains a 1 after the atomic xchg instruction this signifies that another thread already has the lock.

The Stack Overflow answer also mentions [Cache coherence protocol](https://en.wikipedia.org/wiki/Cache_coherence#Coherence_protocols) whose intention is "that two clients must never see different values for the same shared data":

> In effect, the cache coherency protocol prevents other cores from accessing the cache line silently.

[This answer](https://stackoverflow.com/a/14761049/630364) mentions two other technologies: [Load-link/store-conditional](https://en.wikipedia.org/wiki/Load-link/store-conditional) which "are a pair of instructions used in multithreading to achieve synchronization", and [MESI protocol](https://en.wikipedia.org/wiki/MESI_protocol), which is "an Invalidate-based cache coherence protocol".

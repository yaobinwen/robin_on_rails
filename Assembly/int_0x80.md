# System Calls and `int 0x80`

What is a "system call"? Unix systems implement most interfaces between **User Mode** processes and hardware devices by means of "system calls" issued to the kernel.

API vs system calls:
- API: A function definition that specifies how to obtain a given service.
- System call: An explicit request to the **kernel** made via a **software interrupt**.

So "API" is a wider, more general concept than "system call".

When a User Mode process invokes a system call, the CPU switches to Kernel Mode and starts the execution of a kernel function.

Take Ubuntu for example, the system call numbers for x86 can be found in `arch/x86/entry/syscalls`:
- `syscall_32.tbl`: 32-bit system call numbers and entry vectors
- `syscall_64.tbl`: 64-bit system call numbers and entry vectors

The call numbers for the same system call are not always the same on 32- and 64-bit.

Use `find . -name "*syscall*.tbl"` to find all the system call tables:
- `./arch/ia64/kernel/syscalls/syscall.tbl`
- `./arch/sparc/kernel/syscalls/syscall.tbl`
- `./arch/m68k/kernel/syscalls/syscall.tbl`
- `./arch/powerpc/kernel/syscalls/syscall.tbl`
- `./arch/arm/tools/syscall.tbl`
- `./arch/sh/kernel/syscalls/syscall.tbl`
- `./arch/xtensa/kernel/syscalls/syscall.tbl`
- `./arch/microblaze/kernel/syscalls/syscall.tbl`
- `./arch/alpha/kernel/syscalls/syscall.tbl`
- `./arch/x86/entry/syscalls/syscall_64.tbl`
- `./arch/x86/entry/syscalls/syscall_32.tbl`
- `./arch/parisc/kernel/syscalls/syscall.tbl`
- `./arch/mips/kernel/syscalls/syscall_n32.tbl`
- `./arch/mips/kernel/syscalls/syscall_o32.tbl`
- `./arch/mips/kernel/syscalls/syscall_n64.tbl`
- `./arch/s390/kernel/syscalls/syscall.tbl`
- `./tools/perf/arch/powerpc/entry/syscalls/syscall.tbl`
- `./tools/perf/arch/x86/entry/syscalls/syscall_64.tbl`
- `./tools/perf/arch/mips/entry/syscalls/syscall_n64.tbl`
- `./tools/perf/arch/s390/entry/syscalls/syscall.tbl`

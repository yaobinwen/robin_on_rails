# Misc: Many Other Things about Linux

## Apport

I got interested in `apport` on Ubuntu when I was figuring out how to generate core dumps inside a docker container. See my notes of "How to generate core dump inside a container". When I was working on that issue, I was wondering why the core dumps couldn't be generated inside the container but generated on the host machine even if they have the same `/proc/sys/kernel/core_pattern` and `ulimit -c` configuration. Eventually, I realized it may be because of `apport`.

On the host machine I worked with which was installed with `Ubuntu 18.04`, the `/proc/sys/kernel/core_pattern` was `|/usr/share/apport/apport %p %s %c %d %P`. As the "Piping core dumps to a program" section in [1] says:

> Since kernel 2.6.19, ... If the first character of this file is a pipe symbol (|), then the remainder of the line is interpreted as the command-line for a user-space program (or script) that is to be executed.

Therefore, the `core_pattern` file pipes the core dump to `/usr/share/apport/apport` which is a Python script (see [3]) on the host machine. Inside the container that I was using, this script didn't exist. As a result, the core dump was piped to a program that didn't exist. Of course that created nothing.

See [2] for what `Apport` is all about. Basically:

> Apport is a system which:
>
> - intercepts crashes right when they happen the first time,
> - gathers potentially useful information about the crash and the OS environment,
> - can be automatically invoked for unhandled exceptions in other programming languages (e. g. in Ubuntu this is done for Python),
> - can be automatically invoked for other problems that can be automatically detected (e. g. Ubuntu automatically detects and reports package installation/upgrade failures from update-manager),
> - presents a UI that informs the user about the crash and instructs them on how to proceed,
> - and is able to file non-crash bug reports about software, so that developers still get information about package versions, OS version etc.

References:
>
- [1] [CORE(5) man page](https://manpages.ubuntu.com/manpages/bionic/en/man5/core.5.html)
  - Alternatively, run `man core 5` in a terminal.
- [2] [Ubuntu Wiki: Apport](https://wiki.ubuntu.com/Apport)
- [3] [`apport` source code for Ubuntu 18.04](https://git.launchpad.net/ubuntu/+source/apport/tree/data/apport?h=ubuntu/bionic)

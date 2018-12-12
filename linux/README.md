# Linux: Knowledge about the Linux OS

## Overview

[Revolution OS](https://www.youtube.com/watch?v=4vW62KqKJ5A) is a good documentary that "traces the history of GNU, Linux, open source, and the free software movement." Interestingly, [the official website](http://www.revolution-os.com/index.html) doesn't mention the YouTube link at all, but the YouTube video seemed to be uploaded by the official staff.

Although this folder is called "Linux", most of the information is actually for Debian-based systems, unless specified otherwise.

## Books and References

- [Operating Systems: Three Easy Pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/): The book is centered around three conceptual pieces that are fundamental to operating systems: virtualization, concurrency, and persistence.
- [Advanced Programming in the UNIX® Environment](http://www.apuebook.com/)

## Commands

Here is a list of commands I have used in various circumstances. Record them here for future reference.

- Cut text using multi-char delimiter
  - The default `cut` program only accepts one-char delimiter. To use multi-char delimiter, use the following `awk` command:
    `grep pattern file | awk -F "delimiter" '{$0=$1}1'`
  - Reference: [This answer](https://stackoverflow.com/a/25448669/630364).
- `dmidecode`
  - `sudo dmidecode -t16`: Find the information about the memory array.
- `dpkg`:
  - `dpkg -c /path/to/deb/file`: Show the content of the `.deb` file.
  - `dpkg -i /path/to/deb/file`: Install the given `.deb` file.
- `dpkg-query`
  - `dpkg-query -W --showformat='${Version}' dpkg`: Query the version of the installed `dpkg` package.
- `lsmod`: List the currently loaded kernel modules.
- `lspci`
- `lshw`
  - `sudo lshw -c display`: Find all the display devices.
- `modinfo`
  - `modinfo /lib/modules/4.4.0-134-generic/updates/dkms/nvidia_384.ko`: Show the information about `nvidia-384` kernel module.
  - `/lib/modules/<kernel-version>` is where all the kernel files (`.ko`) are placed.
- `uname -a`: Print all the system information.
- `uname -r`: Print the kernel release information.

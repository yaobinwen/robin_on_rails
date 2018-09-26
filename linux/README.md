# Linux: Knowledge about the Linux OS

## Overview

[Revolution OS](https://www.youtube.com/watch?v=4vW62KqKJ5A) is a good documentary that "traces the history of GNU, Linux, open source, and the free software movement." Interestingly, [the official website](http://www.revolution-os.com/index.html) doesn't mention the YouTube link at all, but the YouTube video seemed to be uploaded by the official staff.

Although this folder is called "Linux", most of the information is actually for Debian-based systems, unless specified otherwise.

## Commands

Here is a list of commands I have used in various circumstances. Record them here for future reference.

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

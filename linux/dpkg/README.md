# dpkg

References & tutorials:

- [1] See [Manpages of dpkg in Debian stretch](https://manpages.debian.org/stretch/dpkg/index.html) for all the `dpkg`-related manuals.
- [2] [dpkg(1) manual page](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html)
- [3] [Debian Policy Manual](https://www.debian.org/doc/debian-policy/)
- [4] [Maintainer script flowcharts](https://www.debian.org/doc/debian-policy/#maintainer-script-flowcharts)
- [5] [Chapter 2. Debian package management](https://www.debian.org/doc/manuals/debian-reference/ch02.en.html)
- [6] [Debian Packaging Tutorial](https://www.debian.org/doc/manuals/packaging-tutorial/packaging-tutorial.en.pdf)
- [7] [The Debian Administrator's Handbook](https://debian-handbook.info/download/stable/debian-handbook.pdf)
- [8] [Everything you need to know about conffiles: configuration files managed by dpkg](https://raphaelhertzog.com/2010/09/21/debian-conffile-configuration-file-managed-by-dpkg/)

According to [2], section ["Package states"](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html#Package_states), the entire installation process may consist of two steps:

- Unpack the package.
- Configure the package by putting the [conffiles](https://www.debian.org/doc/manuals/maint-guide/dother.en.html#conffiles) to `/etc`.
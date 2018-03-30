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

---

[Virtual Package](https://www.debian.org/doc/manuals/debian-faq/ch-pkg_basics.en.html#s-virtual) is essentially a functional interface. All the real packages that provide this "virtual package" implement this function. When we say, in the FAQ document, for example, that `exim4` and `sendmail` provide the virtual package `mail-transport-agent`, they actually implement this functionality which is compatible to each other. In fact, the FAQ document itself also says this way:

> Similarly, exim4 and sendmail both **provide the functionality** of a mail transport agent.

---

What is `tasksel`?

A `task` is the functionality that the user wants the machine to have, e.g., an email server, a web server, etc.. The tool `tasksel` provides an interface to specify the functionalities that the user wants to install, and `tasksel` installs all the needed packages to accomplish that task, saving the user's effort to figure out what packages should be installed.

---

What is the "auto/manual install status"? See [this answer](https://askubuntu.com/a/836/514711).

> If you install a package, all packages that that package depends upon are also installed. For example if you install the package `vlc`, it will automatically install `vlc-nox`. The automatically installed packages (in this case, `vlc-nox`) are set as **"automatically installed"** -- if you remove `vlc`, the package manager will suggest to remove `vlc-nox` as well (aptitude will do this automatically, if you use apt-get you can remove all automatically installed packages with apt-get autoremove).
>
> Now, if you do `apt-get install vlc-nox` you will get the message that `vlc-nox` is now set to **"manually installed"**, i.e. the package manager now thinks that you want that package specifically and not just installed it because `vlc` needed it. If you remove `vlc`, `vlc-nox` will therefore not be automatically removed.

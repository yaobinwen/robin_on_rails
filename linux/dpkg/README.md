# dpkg

## Overview

(**NOTE:** This document is still under continuous construction so what you are reading has not met the goal I set for it.)

This folder has the information about Debian/Ubuntu packaging tool set. The **goal** is to make this document a tutorial for people who need to work with Debian packages but have no sense of what the "packaging" is all about.

This document is **task oriented** so I am trying to figure out the common (and uncommon) tasks that people need to do and give guidance of how to do them. I am not creating any new knowledge here. I am collecting the materials that scatter over the Internet and organize them in a way that's easier to understand. All the information can be found in the official documents, but sometimes the official documents are not novice-friendly.

This document is supposed to be **read from top to bottom**, with the sections on the top establishing the foundation to understand the packaging tool ecosystem: what exist there, what they are, how they relate to each other, etc.. Once you get the big picture of the "packaging", the tasks at hand would become a search for specific information.

Here is the brief description of the contained folders and files:

- `yaobin` is an example that shows how to create a package as well as a simple Debian package server. Note this example is not done in the best way as I have been learning more and more about the packaging tools.
  - `yaobin` and `build.py` are the `DEBIAN` files and the build script that create the Debian package.
  - `prep-pkg-repo.py` sets up the simple Debian package server and it uses the GPG keys in `gpgkeys` to sign the packages.
- `dpkg-ecosystem.py` is an atempt to draw the dependency graph of all the Debian packaging tools.

## How to Find A Package

Try on the [official Ubuntu Package Search](https://packages.ubuntu.com/).

- You can either navigate into a specific Ubuntu distribution to find the packages.
  - Click the codename of a distribution.
  - Choose the category of the package you want to find. If you don't know its category, scroll down to the bottom and click `All packages`.
- Alternatively, use the `Search` tool on the page.

Once you get into the page of a specific package, such as the [libboost-filesystem1.65.1](https://packages.ubuntu.com/bionic/libboost-filesystem1.65.1), there is a lot of information:

- The `Other Packages Related to ...` lists the `depends`, `recommends`, `suggests`, and `enhances`.
- The `Download ...` lists the downloadable `.deb` files and their content:
  - Click the corresponding architecture, such as `amd64`, to enter another page that provides the download link.
  - The `Files` is the content of that package, the same as running `dpkg -c` on this package.
- On the right-side panel, the `Download Source Package` lists the packages that contain the **source code** and the **`debian`** files.
  - In the [source package page](https://packages.ubuntu.com/source/bionic/boost1.65.1), scroll down to the bottom to find the download links.

## The `debhelper` Tool Suite

The `debhelper` tool suite is introduced [here](https://manpages.debian.org/stretch/debhelper/debhelper.7.en.html). Note that this is the manpage of Debian _stretch_ which is the latest version as of 2018/08/30. Here is its _Description_ section:

> Debhelper is used to help you build a Debian package. The philosophy behind debhelper is to provide a collection of small, simple, and easily understood tools that are used in debian/rules to automate various common aspects of building a package. This means less work for you, the packager. It also, to some degree means that these tools can be changed if Debian policy changes, and packages that use them will require only a rebuild to comply with the new policy.
>
> A typical debian/rules file that uses debhelper will call several debhelper commands in sequence, or use [dh(1)](https://manpages.debian.org/stretch/debhelper/dh.1.en.html) to automate this process. Examples of rules files that use debhelper are in /usr/share/doc/debhelper/examples/
>
> To create a new Debian package using debhelper, you can just copy one of the sample rules files and edit it by hand. Or you can try the dh-make package, which contains a dh_make command that partially automates the process. For a more gentle introduction, the maint-guide Debian package contains a tutorial about making your first package using debhelper.

## The Other Tools

This list will grow as I learn more about the Debian packaging tools.

### tasksel

A `task` is the functionality that the user wants the machine to have, e.g., an email server, a web server, etc.. The tool `tasksel` provides an interface to specify the functionalities that the user wants to install, and `tasksel` installs all the needed packages to accomplish that task, saving the user's effort to figure out what packages should be installed.

## Setup Debian Package Server

Here are some references:

- [DebianRepository: Setup With Reprepro](https://wiki.debian.org/DebianRepository/SetupWithReprepro)
- [Creating your own Signed APT Repository and Debian Packages](http://blog.jonliv.es/blog/2011/04/26/creating-your-own-signed-apt-repository-and-debian-packages/)
- [15.3. Creating a Package Repository for APT](https://debian-handbook.info/browse/stable/sect.setup-apt-package-repository.html)
- [DebianRepository: Setup](https://wiki.debian.org/DebianRepository/Setup): This link teaches how to set up Debian's official package repository.
- [DebianRepository: Format](https://wiki.debian.org/DebianRepository/Format): This link "documents the structure of the official Debian repository and the format that is officially understood by clients".

## Security

Security is always important. This page, [SecureApt](https://wiki.debian.org/SecureApt), explains in detail how secure apt works and how to use it.

## Miscellaneous

### Virtual Package

[Virtual Package](https://www.debian.org/doc/manuals/debian-faq/ch-pkg_basics.en.html#s-virtual) is essentially a functional interface. All the real packages that provide this "virtual package" implement this function. When we say, in the FAQ document, for example, that `exim4` and `sendmail` provide the virtual package `mail-transport-agent`, they actually implement this functionality which is compatible to each other. In fact, the FAQ document itself also says this way:

> Similarly, exim4 and sendmail both **provide the functionality** of a mail transport agent.

### "auto/manual install status"

What is the "auto/manual install status"? See [this answer](https://askubuntu.com/a/836/514711).

> If you install a package, all packages that that package depends upon are also installed. For example if you install the package `vlc`, it will automatically install `vlc-nox`. The automatically installed packages (in this case, `vlc-nox`) are set as **"automatically installed"** -- if you remove `vlc`, the package manager will suggest to remove `vlc-nox` as well (aptitude will do this automatically, if you use apt-get you can remove all automatically installed packages with apt-get autoremove).
>
> Now, if you do `apt-get install vlc-nox` you will get the message that `vlc-nox` is now set to **"manually installed"**, i.e. the package manager now thinks that you want that package specifically and not just installed it because `vlc` needed it. If you remove `vlc`, `vlc-nox` will therefore not be automatically removed.

## References & tutorials:

- [1] See [Manpages of dpkg in Debian stretch](https://manpages.debian.org/stretch/dpkg/index.html) for all the `dpkg`-related manuals.
- [2] [dpkg(1) manual page](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html)
- [3] [Debian Policy Manual](https://www.debian.org/doc/debian-policy/)
- [4] [Maintainer script flowcharts](https://www.debian.org/doc/debian-policy/ap-flowcharts.html)
- [5] [Chapter 2. Debian package management](https://www.debian.org/doc/manuals/debian-reference/ch02.en.html)
- [6] [Debian Packaging Tutorial](https://www.debian.org/doc/manuals/packaging-tutorial/packaging-tutorial.en.pdf)
- [7] [The Debian Administrator's Handbook](https://debian-handbook.info/download/stable/debian-handbook.pdf)
- [8] [Everything you need to know about conffiles: configuration files managed by dpkg](https://raphaelhertzog.com/2010/09/21/debian-conffile-configuration-file-managed-by-dpkg/)

According to [2], section ["Package states"](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html#Package_states), the entire installation process may consist of two steps:

- Unpack the package.
- Configure the package by putting the [conffiles](https://www.debian.org/doc/manuals/maint-guide/dother.en.html#conffiles) to `/etc`.

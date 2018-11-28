# Packaging

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

## Big Picture of Packaging

If you need to prepare a Debian package, the reference [1] (Debian New Maintainers' Guide) and [2] (Debian Policy Manual) are the official documents for learning all the needed knowledge. Consult them whenever you have a question of how to do a task.

The manual pages of each tool are another official source of information:

- [Debian Manpages](https://manpages.debian.org/)
  - [Here](https://manpages.debian.org/contents-stretch.html) is the link for all the manpages (using `stretch` as an example). Clicking into a tool to see the versions in different languages (if they are available).
- [Ubuntu Manpage Repository](http://manpages.ubuntu.com/)
  - [Here](http://manpages.ubuntu.com/manpages/trusty/) is the link for all the manpages in different languages (using `trusty` as an example). The English version is contained in the folders like `man1`, `man2`, ..., `man9`.

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

**NOTE:** Remember to check the `-updates` repository, such as `trusty-updates` or `bionic-updates`, and see if there is an updated version of the same package. For example, the `libboost1.54_1.54.0-4ubuntu3` is available in `trusty`, but there is an updated version `libboost1.54_1.54.0-4ubuntu3.1` in `trusty-updates`, and usually both `trusty` and `trusty-updates` are configured in the APT source list. Therefore, if you only look at the `trusty` repository, you may overlook the updated package and end up using the wrong version.

## Figure Out Package Dependencies

The tools `debtree` and `apt-rdepends` can help figure out the dependencies of the packages.

`debtree` is briefly introduced [here](https://askubuntu.com/a/261808), and its manpage is [here](http://manpages.ubuntu.com/manpages/bionic/man1/debtree.1.html).

`apt-rdepends`'s manpage is [here](http://manpages.ubuntu.com/manpages/bionic/man1/apt-rdepends.1.html).

## The `debhelper` Tool Suite

The `debhelper` tool suite is introduced [here](https://manpages.debian.org/stretch/debhelper/debhelper.7.en.html). Note that this is the manpage of Debian _stretch_ which is the latest version as of 2018/08/30. Here is its _Description_ section:

> Debhelper is used to help you build a Debian package. The philosophy behind debhelper is to provide a collection of small, simple, and easily understood tools that are used in debian/rules to automate various common aspects of building a package. This means less work for you, the packager. It also, to some degree means that these tools can be changed if Debian policy changes, and packages that use them will require only a rebuild to comply with the new policy.
>
> A typical debian/rules file that uses debhelper will call several debhelper commands in sequence, or use [dh(1)](https://manpages.debian.org/stretch/debhelper/dh.1.en.html) to automate this process. Examples of rules files that use debhelper are in /usr/share/doc/debhelper/examples/
>
> To create a new Debian package using debhelper, you can just copy one of the sample rules files and edit it by hand. Or you can try the dh-make package, which contains a dh_make command that partially automates the process. For a more gentle introduction, the maint-guide Debian package contains a tutorial about making your first package using debhelper.

The section ["4.4.2. Default rules file"](https://www.debian.org/doc/manuals/maint-guide/dreq.en.html#defaultrules) in the "Debian New Maintainers' Guide" explains how these tools are called.

The installed `dh` program is a Perl script which calls other Perl modules. The supporting Perl modules are usually installed into the path `/usr/share/perl5/Debian/Debhelper`.

## The Other Tools

This list will grow as I learn more about the Debian packaging tools.

### apt-cache

[Its manpage](http://manpages.ubuntu.com/manpages/bionic/man8/apt-cache.8.html) says:

> `apt-cache` performs a variety of operations on APT's package cache.  apt-cache does not manipulate the state of the system but does provide operations to search and generate interesting output from the package metadata. The metadata is acquired and updated via the 'update' command of e.g.  apt-get, so that it can be outdated if the last update is too long ago, but in exchange apt-cache works independently of the availability of the configured sources (e.g. offline).

One use of `apt-cache` is to query the version of the installation candidate. For example, running `apt-cache policy gobject-introspection` produces the following output:

```
gobject-introspection:
  Installed: (none)
  Candidate: 1.40.0-1
  Version table:
     1.40.0-1 0
        500 http://archive.ubuntu.com/ubuntu/ trusty/main amd64 Packages
```

There you could see the version of the package if you install it.

### apt-file

As [its manpage](http://manpages.ubuntu.com/manpages/bionic/man1/apt-file.1.html) says:

> apt-file is a command line tool for searching files in packages for the APT package management system.

I came across this tool when I encountered the exactly same problem in [this question](https://stackoverflow.com/q/46958365/630364). It is about a missing file, and the accepted answer uses `apt-file` to locate the package that provides this file.

The simple usage is `apt-file search Sequence/autoreconf.pm`, and it shows the package that contains the file:

> dh-autoreconf: /usr/share/perl5/Debian/Debhelper/Sequence/autoreconf.pm

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

## Backporting

**Warning:** I myself am a novice package backporter so what I am describing below may not be the best practices, or even not completelyl correct.

When you need to backport a package from a newer version of the system (such as Ubuntu 18.04) back to an earlier version (such as Ubuntu 14.04), in general, you need to do the following things.

First of all, find the package's debian files. Refer to the earlier section "How to Find A Package".

Secondly, modify the debian files, especially the `control` file, to use the tools of the version that's available on the target system. For example, the packages on Ubuntu 18.04 usually uses `debhelper (>= 11)` which is not available on Ubuntu 14.04. You may want to lower its version to `(>= 9)`. You may also need to lower the version of the `Standards-Version`. In general, check the entire file for the possible version changes.

However, if its `Build-Depends` specifies another package of the version that's not available on the target system, you may need to consider to backport that package, too, because the current package may call the new APIs in the dependent packages. If you simply lower the version of the dependent package, you may end up with compile errors due to the missing APIs. Even if the package only calls the old APIs, backporting the dependent packages may still be a better idea because the binary compatibility may not be guaranteed.

If the `Build-Depends` only specifies the build tools, it might be fine to simply lower the version of them. However, if the package compilation relies on the new language features of the build tools, especially the compiler, you will have to figure out a workaround.

Use patches to change the code if it doesn't compile.

## Miscellaneous

## Upstream vs Downstream

See this question: [What is the difference between upstream and downstream when referring to who to go to as a developer?](https://askubuntu.com/questions/4868/what-is-the-difference-between-upstream-and-downstream-when-referring-to-who-to)

### Virtual Package

[Virtual Package](https://www.debian.org/doc/manuals/debian-faq/ch-pkg_basics.en.html#s-virtual) is essentially a functional interface. All the real packages that provide this "virtual package" implement this function. When we say, in the FAQ document, for example, that `exim4` and `sendmail` provide the virtual package `mail-transport-agent`, they actually implement this functionality which is compatible to each other. In fact, the FAQ document itself also says this way:

> Similarly, exim4 and sendmail both **provide the functionality** of a mail transport agent.

### "auto/manual install status"

What is the "auto/manual install status"? See [this answer](https://askubuntu.com/a/836/514711).

> If you install a package, all packages that that package depends upon are also installed. For example if you install the package `vlc`, it will automatically install `vlc-nox`. The automatically installed packages (in this case, `vlc-nox`) are set as **"automatically installed"** -- if you remove `vlc`, the package manager will suggest to remove `vlc-nox` as well (aptitude will do this automatically, if you use apt-get you can remove all automatically installed packages with apt-get autoremove).
>
> Now, if you do `apt-get install vlc-nox` you will get the message that `vlc-nox` is now set to **"manually installed"**, i.e. the package manager now thinks that you want that package specifically and not just installed it because `vlc` needed it. If you remove `vlc`, `vlc-nox` will therefore not be automatically removed.

## References & Tutorials

- [1] [Debian New Maintainers' Guide](https://www.debian.org/doc/manuals/maint-guide/)
- [2] [Debian Policy Manual](https://www.debian.org/doc/debian-policy/)
- [3] See [Manpages of dpkg in Debian stretch](https://manpages.debian.org/stretch/dpkg/index.html) for all the `dpkg`-related manuals.
- [4] [dpkg(1) manual page](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html)
- [5] [Maintainer script flowcharts](https://www.debian.org/doc/debian-policy/ap-flowcharts.html)
- [6] [Chapter 2. Debian package management](https://www.debian.org/doc/manuals/debian-reference/ch02.en.html)
- [7] [Debian Packaging Tutorial](https://www.debian.org/doc/manuals/packaging-tutorial/packaging-tutorial.en.pdf)
- [8] [The Debian Administrator's Handbook](https://debian-handbook.info/download/stable/debian-handbook.pdf)
- [9] [Everything you need to know about conffiles: configuration files managed by dpkg](https://raphaelhertzog.com/2010/09/21/debian-conffile-configuration-file-managed-by-dpkg/)

According to [4], section ["Package states"](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html#Package_states), the entire installation process may consist of two steps:

- Unpack the package.
- Configure the package by putting the [conffiles](https://www.debian.org/doc/manuals/maint-guide/dother.en.html#conffiles) to `/etc`.

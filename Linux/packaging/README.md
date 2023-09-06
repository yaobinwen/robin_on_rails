# Packaging

## 1. Overview

(**NOTE:** This document is still under continuous construction so what you are reading has not met the goal I set for it.)

This folder has the information about Debian/Ubuntu packaging tool set. The **goal** is to make this document a tutorial for people who need to work with Debian packages but have no sense of what the "packaging" is all about.

This document is **task oriented** so I am trying to figure out the common (and uncommon) tasks that people need to do and give guidance of how to do them. I am not creating any new knowledge here. I am collecting the materials that scatter over the Internet and organize them in a way that's easier to understand. All the information can be found in the official documents, but sometimes the official documents are not novice-friendly.

This document is supposed to be **read from top to bottom**, with the sections on the top establishing the foundation to understand the packaging tool ecosystem: what exist there, what they are, how they relate to each other, etc.. Once you get the big picture of the "packaging", the tasks at hand would become a search for specific information.

Here is the brief description of the contained folders and files:

- `yaobin` is an example that shows how to create a package as well as a simple Debian package server. Note this example is not done in the best way as I have been learning more and more about the packaging tools.
  - `yaobin` and `build.py` are the `DEBIAN` files and the build script that create the Debian package.
  - `prep-pkg-repo.py` sets up the simple Debian package server and it uses the GPG keys in `gpgkeys` to sign the packages.
- `dpkg-ecosystem.py` is an atempt to draw the dependency graph of all the Debian packaging tools.

## 2. Big Picture of Packaging

If you need to prepare a Debian package, the reference [1] (Debian New Maintainers' Guide) and [2] (Debian Policy Manual) are the official documents for learning all the needed knowledge. Consult them whenever you have a question of how to do a task.

The manual pages of each tool are another official source of information:

- [Debian Manpages](https://manpages.debian.org/)
  - [Here](https://manpages.debian.org/contents-stretch.html) is the link for all the manpages (using `stretch` as an example). Clicking into a tool to see the versions in different languages (if they are available).
- [Ubuntu Manpage Repository](http://manpages.ubuntu.com/)
  - [Here](http://manpages.ubuntu.com/manpages/trusty/) is the link for all the manpages in different languages (using `trusty` as an example). The English version is contained in the folders like `man1`, `man2`, ..., `man9`.

## 3. How to Find A Package

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

## 4. Figure Out Package Dependencies

The tools `debtree` and `apt-rdepends` can help figure out the dependencies of the packages.

`debtree` is briefly introduced [here](https://askubuntu.com/a/261808), and its manpage is [here](http://manpages.ubuntu.com/manpages/bionic/man1/debtree.1.html).

`apt-rdepends`'s manpage is [here](http://manpages.ubuntu.com/manpages/bionic/man1/apt-rdepends.1.html).

## 5. How to download the source code of an official package?

The official Ubuntu Package Server provides the websites of the packages where you can download the source code. However, the easier way is to use `apt-get source` to download the package's source code. The benefit of this method is you are guaranteed to download the source code that's used to build the executable you use on Ubuntu.

Follow the steps below to use `apt-get source` to download the source code:
- 1). Edit `/etc/apt/sources.list` to enable the source code repositories, i.e., those starting with `deb-src`. For example:
```
# See http://help.ubuntu.com/community/UpgradeNotes for how to upgrade to
# newer versions of the distribution.
deb http://us.archive.ubuntu.com/ubuntu/ bionic main restricted
deb-src http://us.archive.ubuntu.com/ubuntu/ bionic main restricted
```
- 2). Run `sudo apt-get update` to update the index.
- 3). Run `apt-get source <package-name>` to download the source code and apply the patches automatically. Take `netcat` for example. Running `apt-get source netcat` does the following things automatically:
  - a). Download the source tarball `netcat_1.10.orig.tar.bz2` in the current directory. (The command also downloads the [`.dsc` file](https://www.debian.org/doc/debian-policy/ch-controlfields.html#s-debiansourcecontrolfiles) which describes the source package.)
  - b). Download the `debian` tarball `netcat_1.10-41.1.debian.tar.xz` in the current directory.
  - c). Extract the source code from the source tarball into the current directory.
  - d). Extract the `debian` packaging files from the `debian` tarball and put them into the extracted source code directory automatically.
  - e). Apply the `debian/patches` automatically.
- 4). Now the code is ready to be built. Enter the source code directory and run `debuild -us -uc` can build the code.

Regarding `apt-get source`: The default behavior is to download and unpack the source tarball to make it ready for building. However, you can specify two CLI options to change the behavior:
- 1). `--download-only` (e.g., `apt-get source netcat --download-only`) to only download the source tarball, the `.dsc` file, and the `debian` tarball.
- 2). `--build` (e.g., `apt-get source netcat --build`) to download, unpack, and also build the source code.

## 6. How to manually apply the patches to the source code?

Once you download the source code (see section 5 above), you can apply the patches to see how the official Ubuntu release deals with the source code. There are two ways to apply the patches:
- 1). Run `dpkg-source --extract <filename.dsc> [output-directory]` (e.g., `dpkg-source --extract netcat_1.10-41.1.dsc netcat-1.10`).
  - a). You can also run `dpkg-source --no-check --extract <filename.dsc> [output-directory]` if you don't want to verify the signature in the `.dsc` file.
- 2). If you want to apply the patches one by one, you can follow the steps below:
  - a). Unpack the source tarball and the `debian` tarball.
  - b). Move the `debian` folder into the source code folder. So the folder structure should look like the following (using `netcat` as an example):
```
├── netcat-1.10 (<== Extracted source code folder)
│   ├── Changelog
│   ├── data
│   ├── debian (<== The `debian` folder is moved here)
│   ├── generic.h
│   ├── Makefile
│   ├── netcat.blurb
│   ├── netcat.c
│   ├── README
│   ├── scripts
│   └── stupidh
├── netcat_1.10-41.1.debian.tar.xz
├── netcat_1.10-41.1.dsc
├── netcat_1.10.orig.tar.bz2
```
  - c). `cd` into the source code folder, such as `cd netcat-1.10`.
  - d). Run `patch -p1 ./debian/patches/<patch-file-name>` to apply the specific patch to the source code.

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

### dpkg

See [4] for more details. Here is a list of frequently used commands.

- `dpkg`:
  - `dpkg -c /path/to/deb/file`: Show the content of the `.deb` file.
  - `dpkg -i /path/to/deb/file`: Install the given `.deb` file.
- `dpkg-query`
  - `dpkg-query -W --showformat='${Version}' pkg_name`: Query the version of the installed `pkg_name` package.

### tasksel

A `task` is the functionality that the user wants the machine to have, e.g., an email server, a web server, etc.. The tool `tasksel` provides an interface to specify the functionalities that the user wants to install, and `tasksel` installs all the needed packages to accomplish that task, saving the user's effort to figure out what packages should be installed.

## Setup Debian Package Server

Here are some references:

- [DebianRepository: Setup With Reprepro](https://wiki.debian.org/DebianRepository/SetupWithReprepro)
- [Creating your own Signed APT Repository and Debian Packages](http://blog.jonliv.es/blog/2011/04/26/creating-your-own-signed-apt-repository-and-debian-packages/)
- [15.3. Creating a Package Repository for APT](https://debian-handbook.info/browse/stable/sect.setup-apt-package-repository.html)
- [DebianRepository: Setup](https://wiki.debian.org/DebianRepository/Setup): This link teaches how to set up Debian's official package repository.
- [DebianRepository: Format](https://wiki.debian.org/DebianRepository/Format): This link "documents the structure of the official Debian repository and the format that is officially understood by clients".

Specifically, to set up a package server that's in a local directory, use the following repository file format: `deb [trusted=yes] file:///path/to/archive/root ./`

Here we are using the [Flat Repository Format](https://wiki.debian.org/DebianRepository/Format#Flat_Repository_Format) which has a general form: `deb uri directory/` and you must provide the `Packages` and `Sources` file.

A `Packages` file can be generated using `dpkg-scanpackages`:

```shell
cd /path/to/archive/root
dpkg-scanpackages . >Packages
cp -pv Packages Sources
```

## Security

Security is always important. This page, [SecureApt](https://wiki.debian.org/SecureApt), explains in detail how secure apt works and how to use it.

## Backporting

**Warning:** I myself am a novice package backporter so what I am describing below may not be the best practices, or even not completelyl correct.

When you need to backport a package from a newer version of the system (such as Ubuntu 18.04) back to an earlier version (such as Ubuntu 14.04), in general, you need to do the following things.

First of all, find the package's debian files. Refer to the earlier section "How to Find A Package".

Secondly, modify the debian files, especially the `control` file, to use the tools of the version that's available on the target system. For example, the packages on Ubuntu 18.04 usually uses `debhelper (>= 11)` which is not available on Ubuntu 14.04. You may want to lower its version to `(>= 9)`. You may also need to lower the version of the `Standards-Version`. In general, check the entire file for the possible version changes.

However, if its `Build-Depends` specifies another package of the version that's not available on the target system, you may need to consider to backport that package, too, because the current package may call the new APIs in the dependent packages. If you simply lower the version of the dependent package, you may end up with compile errors due to the missing APIs. Even if the package only calls the old APIs, backporting the dependent packages may still be a better idea because the binary compatibility may not be guaranteed.

If the `Build-Depends` only specifies the build tools, it might be fine to simply lower the version of them. However, if the package compilation relies on the new language features of the build tools, especially the compiler, you will have to figure out a workaround.

Use patches to change the code if it doesn't compile. You will need to use `quilt` to create the patch. Refer to [1] for [how to set up `quilt` and create the patches](https://www.debian.org/doc/manuals/maint-guide/modify.en.html).

## Installation

### Deal with Interaction

Sometimes, a package prompts the user with questions that the user must answer manually in order to proceed with the installation. This is implemented using `debconf` which is "is the preferred mechanism used in Ubuntu to interact with the user when configuring packages and also forms the heart of debian-installer" [10]. [10] has a section that talks about "preseeding" which is the way that installs a package automatically.

Generally speaking, `debconf` works this way:

- Figure out the interactions that the package requires. This can be done in several ways:
  - Look for the `template` file in the `.deb` file which has all the interactions.
  - Manually install the package and view the `template` content in `/var/lib/dpkg/info/<package-name>.list`.
- Work out a `preseed.cfg` file that contains the pre-selected answers to the interactions. See [10] for how to make this file.
- Before the package installation starts, use `debconf-set-selections` to set the answers.
- `debconf` stores all the answers in the file `/var/cache/debconf/config.dat`. You can directly view its content, or use `debconf-show` to query its content. The actual path of `config.dat` can be found in `/var/lib/dpkg/info/debconf.list`.
- Run `apt` to install the package. If everything works correctly, you wouldn't see any interaction.

## Packaging Python Modules

Python and its modules seem to be important enough that the Debian community has a _"Debian Python Policy"_(see [11]) and a _"Python Library Style Guide"_(see [12]). [11] talks about Python distribution in general and [12] is more specific about packaging and distributing a Python module.

A less common but occurring issue is: A module may provide an executable script `foo.py` which is Python 2 and 3 compatible that wants to be installed to the same location (say, `/usr/bin`). Although we can still work out the `python-foo` and `python3-foo` packages, they can't be installed at the same time because, if you have installed `python-foo`, the installation of `python3-foo` would fail because of an error like this:

```
dpkg: error processing archive /var/cache/apt/archives/python3-foo_1.2.3-1_all.deb (--unpack):
 trying to overwrite '/usr/bin/foo.py', which is also in package python-foo_1.2.3-1
```

The section "Executables and library packages" in [12] talks about a similar issue. Go take a look.

A similar solution is: keep the name `foo.py` for Python 2, and rename the script to `foo3.py` for Python 3. Both can be installed at the same location `/usr/bin`.

The article [Debian packaging for python2 and python3 at the same time](https://www.v13.gr/blog/?p=412) provides another solution. It uses the trick of `export PYBUILD_INSTALL_ARGS_python2 = --install-scripts=/dev/null` to avoid installing the Python 2 version and only installs the Python 3 version.

## Debian Go Packaging

See [Debian Go Packaging](https://go-team.pages.debian.net/packaging.html). Some mentioned tools:
- [dh-make-golang](https://github.com/Debian/dh-make-golang): tool that converts Go packages into Debian package source.
- [dh-golang](https://packages.ubuntu.com/bionic/devel/dh-golang): debhelper add-on for packaging software written in Go (golang).

## Miscellaneous

### Upstream vs Downstream

See this question: [What is the difference between upstream and downstream when referring to who to go to as a developer?](https://askubuntu.com/questions/4868/what-is-the-difference-between-upstream-and-downstream-when-referring-to-who-to)

### Virtual Package

[Virtual Package](https://www.debian.org/doc/manuals/debian-faq/ch-pkg_basics.en.html#s-virtual) is essentially a functional interface. All the real packages that provide this "virtual package" implement this function. When we say, in the FAQ document, for example, that `exim4` and `sendmail` provide the virtual package `mail-transport-agent`, they actually implement this functionality which is compatible to each other. In fact, the FAQ document itself also says this way:

> Similarly, exim4 and sendmail both **provide the functionality** of a mail transport agent.

### "auto/manual install status"

What is the "auto/manual install status"? See [this answer](https://askubuntu.com/a/836/514711).

> If you install a package, all packages that that package depends upon are also installed. For example if you install the package `vlc`, it will automatically install `vlc-nox`. The automatically installed packages (in this case, `vlc-nox`) are set as **"automatically installed"** -- if you remove `vlc`, the package manager will suggest to remove `vlc-nox` as well (aptitude will do this automatically, if you use apt-get you can remove all automatically installed packages with apt-get autoremove).
>
> Now, if you do `apt-get install vlc-nox` you will get the message that `vlc-nox` is now set to **"manually installed"**, i.e. the package manager now thinks that you want that package specifically and not just installed it because `vlc` needed it. If you remove `vlc`, `vlc-nox` will therefore not be automatically removed.

### Creating a Patch

Use `quilt` to patch the source code. The [_Chapter 3. Modifying the source_](https://www.debian.org/doc/manuals/maint-guide/modify.en.html) provides the instructions of setting up `quilt` and creating the patch.

An alternative way of creating the patch is:

- 1). Run `apt source package-name` to download the `debian` files and the source code.
- 2). Change into the source code directory (which also has the `debian` directory).
- 3). Modify the source code.
- 4). Run `dpkg-source --commit` to "store upstream changes in a new patch" (see its manual).
  - Note: Some source format doesn't support `--commit`.

### `shlibs` files

The format of the files can be found in [`deb-shlibs(5)`](https://man7.org/linux/man-pages/man5/deb-shlibs.5.html):

> There is one entry per line. Blank lines are not allowed.  Lines beginning with an `#` character are considered commentary, and are ignored.
>
> All other lines must have the format
>
> `[type:] library version dependencies`
>
> Example:
>
> `libcrunch 1 libcrunch1 (>= 1.2-1)`

Also see my question: [_What is the detailed process of resolving `${shlibs:Depends}` when creating a Debian package?_](https://askubuntu.com/q/1086226/514711).

## Reproducible Builds

See [Reproducible Builds](https://reproducible-builds.org/).

The [Continuous tests](https://reproducible-builds.org/citests/) page lists the test results of all the monitored operating systems. The one I am interested in is [for Debian](https://tests.reproducible-builds.org/debian/reproducible.html). See its [Categorized issues](https://tests.reproducible-builds.org/debian/index_issues.html) for all the possible issues about build reproducibility.

## FAQ

### Q1: What does 'patch unexpectedly ends in middle of line' mean?

See [this answer](https://unix.stackexchange.com/a/1403/162971) which refers to other links:

- [Subject: patch file problem](http://web.archive.org/web/20091030150511/http://support.github.com/discussions/repos/1784-patch-file-problem)
- [Configuring Git to handle line endings](https://docs.github.com/en/github/using-git/configuring-git-to-handle-line-endings)

"It is about patch unexpectedly ends in middle of line messages because of CRLF (carriage-return, linefeed)."

Although the discussion focuses on git, the issue is also applicable to `debian/patches`: I once created a patch using `dpkg-source --commit` in a VM but then copied-and-pasted its content to the patch file on the host machine. As a result, the line endings were changed without me realizing it and resulted in the "patch unexpectedly ends in middle of line" error. I didn't get it fixed until I used the patch files themselves.

## References & Tutorials

- [1] [Debian New Maintainers' Guide](https://www.debian.org/doc/manuals/maint-guide/)
  - As of 2021-01-25, the [Guide for Debian Maintainers](https://www.debian.org/doc/manuals/debmake-doc/) is "considered as the successor to the _Debian New Maintainers' Guide_."
- [2] [Debian Policy Manual](https://www.debian.org/doc/debian-policy/)
- [3] See [Manpages of dpkg in Debian stretch](https://manpages.debian.org/stretch/dpkg/index.html) for all the `dpkg`-related manuals.
- [4] [dpkg(1) manual page](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html)
- [5] [Maintainer script flowcharts](https://www.debian.org/doc/debian-policy/ap-flowcharts.html)
- [6] [Chapter 2. Debian package management](https://www.debian.org/doc/manuals/debian-reference/ch02.en.html)
- [7] [Debian Packaging Tutorial](https://www.debian.org/doc/manuals/packaging-tutorial/packaging-tutorial.en.pdf)
- [8] [The Debian Administrator's Handbook](https://debian-handbook.info/download/stable/debian-handbook.pdf)
- [9] [Everything you need to know about conffiles: configuration files managed by dpkg](https://raphaelhertzog.com/2010/09/21/debian-conffile-configuration-file-managed-by-dpkg/)
- [10] [Ubuntu Installation Guide: B. Automating the installation using preseeding](https://help.ubuntu.com/lts/installation-guide/armhf/apb.html)
- [11] [Debian Python Policy](https://www.debian.org/doc/packaging-manuals/python-policy/index.html)
- [12] [Python Library Style Guide](https://wiki.debian.org/Python/LibraryStyleGuide)

According to [4], section ["Package states"](https://manpages.debian.org/stretch/dpkg/dpkg.1.en.html#Package_states), the entire installation process may consist of two steps:

- Unpack the package.
- Configure the package by putting the [conffiles](https://www.debian.org/doc/manuals/maint-guide/dother.en.html#conffiles) to `/etc`.

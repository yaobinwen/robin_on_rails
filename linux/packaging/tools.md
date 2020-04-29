# Tools

## Overview

The Debian packaging uses a lot of tools that can confuse people and make them doubt what tools are for what tasks. This document tries to list all the tools in order to provide a big picture. The packages are ordered alphabetically.

Although the packages are about "Debian" packaging, I'm using [`Ubuntu Package Server`](https://packages.ubuntu.com/) and `18.04` as the primary reference.

I've cloned the repositories for a learning purpose:

- [`dpkg`](https://github.com/yaobinwen/dpkg)

## debhelper

[`debhelper`](https://packages.ubuntu.com/bionic/debhelper) is the set of "helper programs for debian/rules". It installs the following programs:

- /usr/bin/dh
- /usr/bin/dh_auto_build
- /usr/bin/dh_auto_clean
- /usr/bin/dh_auto_configure
- /usr/bin/dh_auto_install
- /usr/bin/dh_auto_test
- /usr/bin/dh_bugfiles
- /usr/bin/dh_builddeb
- /usr/bin/dh_clean
- /usr/bin/dh_compress
- /usr/bin/dh_dwz
- /usr/bin/dh_fixperms
- /usr/bin/dh_gconf
- /usr/bin/dh_gencontrol
- /usr/bin/dh_icons
- /usr/bin/dh_install
- /usr/bin/dh_installcatalogs
- /usr/bin/dh_installchangelogs
- /usr/bin/dh_installcron
- /usr/bin/dh_installdeb
- /usr/bin/dh_installdebconf
- /usr/bin/dh_installdirs
- /usr/bin/dh_installdocs
- /usr/bin/dh_installemacsen
- /usr/bin/dh_installexamples
- /usr/bin/dh_installgsettings
- /usr/bin/dh_installifupdown
- /usr/bin/dh_installinfo
- /usr/bin/dh_installinit
- /usr/bin/dh_installlogcheck
- /usr/bin/dh_installlogrotate
- /usr/bin/dh_installman
- /usr/bin/dh_installmanpages
- /usr/bin/dh_installmenu
- /usr/bin/dh_installmime
- /usr/bin/dh_installmodules
- /usr/bin/dh_installpam
- /usr/bin/dh_installppp
- /usr/bin/dh_installsystemd
- /usr/bin/dh_installudev
- /usr/bin/dh_installwm
- /usr/bin/dh_installxfonts
- /usr/bin/dh_link
- /usr/bin/dh_lintian
- /usr/bin/dh_listpackages
- /usr/bin/dh_makeshlibs
- /usr/bin/dh_md5sums
- /usr/bin/dh_missing
- /usr/bin/dh_movefiles
- /usr/bin/dh_perl
- /usr/bin/dh_prep
- /usr/bin/dh_shlibdeps
- /usr/bin/dh_strip
- /usr/bin/dh_systemd_enable
- /usr/bin/dh_systemd_start
- /usr/bin/dh_testdir
- /usr/bin/dh_testroot
- /usr/bin/dh_ucf
- /usr/bin/dh_update_autotools_config
- /usr/bin/dh_usrlocal

## devscripts

[`devscripts`](https://packages.ubuntu.com/bionic/devscripts) is the set of "scripts to make the life of a Debian Package maintainer easier". It installs the following programs:

- /usr/bin/add-patch
- /usr/bin/annotate-output
- /usr/bin/archpath
- /usr/bin/bts
- /usr/bin/build-rdeps
- /usr/bin/chdist
- /usr/bin/checkbashisms
- /usr/bin/cowpoke
- /usr/bin/cvs-debc
- /usr/bin/cvs-debi
- /usr/bin/cvs-debrelease
- /usr/bin/cvs-debuild
- /usr/bin/dch
- /usr/bin/dcmd
- /usr/bin/dcontrol
- /usr/bin/dd-list
- /usr/bin/deb-reversion
- /usr/bin/debc
- /usr/bin/debchange
- /usr/bin/debcheckout
- /usr/bin/debclean
- /usr/bin/debcommit
- /usr/bin/debdiff
- /usr/bin/debdiff-apply
- /usr/bin/debi
- /usr/bin/debpkg
- /usr/bin/debrelease
- /usr/bin/debrepro
- /usr/bin/debrsign
- /usr/bin/debsign
- /usr/bin/debsnap
- /usr/bin/debuild
- /usr/bin/dep3changelog
- /usr/bin/desktop2menu
- /usr/bin/dget
- /usr/bin/diff2patches
- /usr/bin/dpkg-depcheck
- /usr/bin/dpkg-genbuilddeps
- /usr/bin/dscextract
- /usr/bin/dscverify
- /usr/bin/edit-patch
- /usr/bin/getbuildlog
- /usr/bin/git-deborig
- /usr/bin/grep-excuses
- /usr/bin/hardening-check
- /usr/bin/list-unreleased
- /usr/bin/ltnu
- /usr/bin/manpage-alert
- /usr/bin/mass-bug
- /usr/bin/mergechanges
- /usr/bin/mk-build-deps
- /usr/bin/mk-origtargz
- /usr/bin/namecheck
- /usr/bin/nmudiff
- /usr/bin/origtargz
- /usr/bin/plotchangelog
- /usr/bin/pts-subscribe
- /usr/bin/pts-unsubscribe
- /usr/bin/rc-alert
- /usr/bin/reproducible-check
- /usr/bin/rmadison
- /usr/bin/sadt
- /usr/bin/suspicious-source
- /usr/bin/svnpath
- /usr/bin/tagpending
- /usr/bin/transition-check
- /usr/bin/uscan
- /usr/bin/uupdate
- /usr/bin/what-patch
- /usr/bin/who-permits-upload
- /usr/bin/who-uploads
- /usr/bin/whodepends
- /usr/bin/wnpp-alert
- /usr/bin/wnpp-check
- /usr/bin/wrap-and-sort

## dh-make

[`dh-make`](https://packages.ubuntu.com/bionic/dh-make) is the "tool that converts source archives into Debian package source". It installs the following programs:

- [/usr/bin/dh_make](https://manpages.debian.org/jessie/dh-make/dh_make.8.en.html)
- /usr/bin/dh_makefont

## dpkg

[`dpkg`](https://packages.ubuntu.com/bionic/dpkg) is the "Debian package management system". It installs the following programs:

- /sbin/start-stop-daemon
- /usr/bin/dpkg
- /usr/bin/dpkg-deb
- /usr/bin/dpkg-divert
- /usr/bin/dpkg-maintscript-helper
- /usr/bin/dpkg-query
- /usr/bin/dpkg-split
- /usr/bin/dpkg-statoverride
- /usr/bin/dpkg-trigger
- /usr/bin/update-alternatives

## dpkg-dev

[`dpkg-dev`](https://packages.ubuntu.com/bionic/dpkg-dev) is the "Debian package development tools". It installs the following programs:

- /usr/bin/dpkg-architecture
- /usr/bin/dpkg-buildflags
- /usr/bin/dpkg-buildpackage
- /usr/bin/dpkg-checkbuilddeps
- /usr/bin/dpkg-distaddfile
- /usr/bin/dpkg-genbuildinfo
- /usr/bin/dpkg-genchanges
- /usr/bin/dpkg-gencontrol
- /usr/bin/dpkg-gensymbols
- /usr/bin/dpkg-mergechangelogs
- /usr/bin/dpkg-name
- /usr/bin/dpkg-parsechangelog
- /usr/bin/dpkg-scanpackages
- /usr/bin/dpkg-scansources
- /usr/bin/dpkg-shlibdeps
- /usr/bin/dpkg-source
- /usr/bin/dpkg-vendor

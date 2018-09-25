# README

## Overview

This document contains the knowledge about the technologies about graphics card and drivers.

The information below is currently specific to the following work environment:

- Dell series notebooks.
- Ubuntu 14.04.5 (kernel version: 4.4.0-31-generic; 4.4.0-135-generic).
- NVIDIA graphics cards.

## Install NVIDIA Driver

This post, [Install NVIDIA Driver and CUDA on Ubuntu / CentOS / Fedora Linux OS ](https://gist.github.com/yaobinwen/d244bbd64e6fc8fab2b7e5a47cae497a)(referred to as "[Install]"), is a good start.

How to find the official NVIDIA graphics card driver:

- The [NVIDIA Driver Downloads](https://www.nvidia.com/Download/index.aspx) is the official selector to find the latest driver.
  - Note that if you are using a notebook rather than desktop PC, please choose the `(Notebook)` entries in the `Product Series`.
  - The driver downloaded from this website is an executable `.run` file which is a combination of Shell script and binary content (which is the actual compressed driver).
  - Refer to [NVIDIA Accelerated Linux Graphics Driver README and Installation Guide](http://us.download.nvidia.com/XFree86/Linux-x86/173.14.12/README/index.html) for the detailed installation instructions.
    - [Chapter 4. Installing the NVIDIA Driver](http://us.download.nvidia.com/XFree86/Linux-x86/173.14.12/README/chapter-04.html) is specificially for installation.
- As "[Install]" points out, the Debian package on the the official Ubuntu repository may not always be the latest. As time of this writing, the latest version is `nvidia-384`.

Other considerations include:

- The `Secure Boot` feature needs to be disabled so the NVIDIA graphics driver (which is counted as a third-party one) can be loaded.
- The `Switchable Graphics` features needs to be disabled if there are multiple graphics cards installed in the computer.
- As "[Install]" points out, if you use the `nvidia-384` Debian package to install the driver, you need to re-install it after you upgrade the Linux kernel (such as from `4.4.0-31` to `4.4.0-135`).
- If the kernel is compiled with `retpoline` patch, you also need to use more recent `g++` compilers (`g++-7` at least) so it supports the `retpoline` compilation for the driver's kernel module. Otherwise you'll get mismatched version line error in `dmesg`.

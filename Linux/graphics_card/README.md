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

## Nvidia Optimus

According to [2]:

> NVIDIA® Optimus™ technology intelligently optimizes your notebook PC, providing the outstanding graphics performance you need, when you need it, all the while extending battery life for longer enjoyment.

According to the "Executive Summary" of [3]:

> As a result, a few years ago NVIDIA developed a technology dubbed **"switchable graphics"** which allowed the end-user to choose which display adaptor would be used. In essence, this technology brought the best of both worlds, as it offered the battery life of an integrated graphics solution and the performance of a discrete GPU. Unfortunately, there were limitations to the technology resulting in the end user having to execute a fairly involved procedure to harness the most from the platform, resulting in only 1% of users ever switching between the two graphics systems.
>
> With NVIDIA's new Optimus technology, users can now experience the full performance benefits of a discrete GPU with the battery life of an integrated graphics solution. NVIDIA Optimus automatically, instantaneously, and seamlessly optimizes the notebook to offer the best performance or best battery life depending on the application.

Other things I have learned from the reading:

- "Integrated graphics had previously been located on motherboards, but recent generations of Intel chips (and AMD 'APU' chips) place the integrated graphics chip on the same package as the CPU. Hence, the motherboard can support integrated graphics if the CPU has that chip but not all newer Intel chips have this feature." [5]

The reference [3], the Optimus whitepaper is a **great** source of information for both the background and the technological details.

References:

- [1] [A guide to Nvidia Optimus on Dell PCs with an Ubuntu Operating System](https://www.dell.com/support/article/ba/en/babsdt1/sln298431/a-guide-to-nvidia-optimus-on-dell-pcs-with-an-ubuntu-operating-system?lang=en)
- [2] [Nvidia: Optimus Technology](https://www.nvidia.com/object/optimus_technology.html)
- [3] [Optimus Whitepaper: A technical description of Optimus Technology and how it works.](https://www.nvidia.com/object/LO_optimus_whitepapers.html)
- [4] [GeForce: Optimus](https://www.geforce.com/hardware/technology/optimus)
- [5] [CPU: integrated graphics?](http://www.tomshardware.com/answers/id-1820282/cpu-integrated-graphics.html#r11627188)

## DRM(Direct Rendering Manager) and KMS(Kernel Mode Setting)

References:

- [1] [Direct Rendering Manager](https://en.wikipedia.org/wiki/Direct_Rendering_Manager)
- [2] [Kernel Mode Setting: ArchLinux](https://wiki.archlinux.org/index.php/kernel_mode_setting)
- [3] [Kernel Mode Setting: Debian Wiki](https://wiki.debian.org/KernelModesetting)

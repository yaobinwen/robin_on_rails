# README

## Overview

This document is all about desktop environments on various Linux distributions. This is necessary because, unlike Windows or Mac, Linux comes with a variety of desktop environments which can overwhelm a novice user.

## Desktop Environments

According to [Wikipedia](https://en.wikipedia.org/wiki/Desktop_environment), a `desktop environment (DE)` is "an implementation of the desktop metaphor made of a bundle of programs running on top of a computer operating system, which share a common _graphical user interface (GUI)_, sometimes described as a _graphical shell_."

The point is: `DE` is different from `GUI`.

The article [Best Linux Desktop Environment: 15 Reviewed and Compared](https://www.ubuntupit.com/best-linux-desktop-environment-reviewed-and-compared/) lists many desktop environments (DE). Some of them that I should care about are:

- [GNOME](https://www.gnome.org/): Default DE for "Fedora, Debian, Ubuntu, SUSE Linux Enterprise (exclusively), Red Hat Enterprise Linux, CentOS, Oracle Linux, Scientific Linux, SteamOS, Tails, Kali Linux, Antergos and Endless OS" (see [Wiki](https://en.wikipedia.org/wiki/GNOME)).
- [KDE](https://www.kde.org/): Technically, KDE is not a desktop environment but a community that develops free and open source software. Here is [a list of their projects](https://community.kde.org/Main_Page). [`KDE Plasma`](https://www.kde.org/plasma-desktop) is the actual DE.
- [Unity](https://unity8.io/): "Unity is a graphical shell for the GNOME desktop environment, ... Unlike GNOME, KDE Software Compilation, Xfce, or LXDE, Unity is not a collection of applications but is designed to use existing programs."(from [Wiki](https://en.wikipedia.org/wiki/Unity_(user_interface))).
  - According to the question ["Difference between Unity and GNOME"](https://askubuntu.com/questions/333237/difference-between-unity-and-gnome), `Unity` replaces the default `GNOME shell` on Ubuntu distributions.
  - `Unity` was the graphical shell for the versions between Ubuntu 10 and 17. Ubuntu 18.04 swtiches back to GNOME 3 (with the native graphical shell).

## GNOME

[`gsettings`](https://wiki.gnome.org/HowDoI/GSettings) is the tool to configure application settings on GNOME. `GSettings` can be seen as a database so it has the concept of `schemas`, `keys` and `values`.

Here is basic information about [`gconf`](https://en.wikipedia.org/wiki/GConf):

> GConf was a system used by the GNOME desktop environment for storing configuration settings for the desktop and applications. It is similar to the Windows Registry.
>
> It was deprecated as part of the GNOME 3 transition. Migration to its replacement, GSettings and dconf, is ongoing.

Here is basic information about [`dconf`](https://en.wikipedia.org/wiki/Dconf):

> dconf is a low-level configuration system and settings management. Its main purpose is to provide a back end to GSettings on platforms that don't already have configuration storage systems. It depends on GLib. It is part of GNOME 3 and is a replacement for GConf.

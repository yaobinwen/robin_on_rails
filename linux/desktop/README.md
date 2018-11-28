# README

## Overview

This document is all about desktop environments on various Linux distributions. This is necessary because, unlike Windows or Mac, Linux comes with a variety of desktop environments which can overwhelm a novice user.

## Big Picture

[This answer](https://unix.stackexchange.com/a/464321/162971) is an **incredible** one that provides both the **big picture** and the **details** that allows the reader to understand the topic and context. Check it out before reading the following. Here is the picture used in this answer (the picture is used in the Wikipedia entry for [Display server]((https://en.wikipedia.org/wiki/Display_server))):

![Full GUI Stack](https://i.stack.imgur.com/EpnVe.png)

## X Window System

See the Wikipedia entry: [X Window System](https://en.wikipedia.org/wiki/X_Window_System). The main points are listed as follows.

`X Window System`, or `X`, is a networked windowing system.

That being _networked_ means `X` can help both local and remote applications to render graphical elements.

Its networking capability is in a client-server architecture, which means `X` has its own communication protocol, `X protocol`, for the clients and server. The current (as of 2018/11/28) version of `X protocol` is 11, hence `X11`.

`X.Org Foundation` is the organization that leads the `X` project.

`X.Org Server` is the ["free and open-source implementation of the display server for the X Window System."](https://en.wikipedia.org/wiki/X.Org_Server)

The Wikipedia article ["Purpose and abilities"](https://en.wikipedia.org/wiki/X_Window_System#Purpose_and_abilities) says this about an X server and a client:

> The fact that the term "server" is applied to the software in front of the user is often surprising to users accustomed to their programs being clients to services on remote computers. Here, rather than a remote database being the resource for a local app, **the user's graphic display and input devices become resources made available by the local X server to both local and remotely hosted X client programs** who need to share the user's graphics and input devices to communicate with the user.

## Desktop Environments

According to [Wikipedia](https://en.wikipedia.org/wiki/Desktop_environment), a `desktop environment (DE)` is "an implementation of the desktop metaphor made of a bundle of programs running on top of a computer operating system, which share a common _graphical user interface (GUI)_, sometimes described as a _graphical shell_."

The point is: `DE` is different from `GUI`.

The article [Best Linux Desktop Environment: 15 Reviewed and Compared](https://www.ubuntupit.com/best-linux-desktop-environment-reviewed-and-compared/) lists many desktop environments (DE). Some of them that I should care about are:

- [GNOME](https://www.gnome.org/): Default DE for "Fedora, Debian, Ubuntu, SUSE Linux Enterprise (exclusively), Red Hat Enterprise Linux, CentOS, Oracle Linux, Scientific Linux, SteamOS, Tails, Kali Linux, Antergos and Endless OS" (see [Wiki](https://en.wikipedia.org/wiki/GNOME)).
- [KDE](https://www.kde.org/): Technically, KDE is not a desktop environment but a community that develops free and open source software. Here is [a list of their projects](https://community.kde.org/Main_Page). [`KDE Plasma`](https://www.kde.org/plasma-desktop) is the actual DE.
- [Unity](https://unity8.io/): "Unity is a graphical shell for the GNOME desktop environment, ... Unlike GNOME, KDE Software Compilation, Xfce, or LXDE, Unity is not a collection of applications but is designed to use existing programs."(from [Wiki](https://en.wikipedia.org/wiki/Unity_(user_interface))).
  - According to the question ["Difference between Unity and GNOME"](https://askubuntu.com/questions/333237/difference-between-unity-and-gnome), `Unity` replaces the default `GNOME shell` on Ubuntu distributions.
  - `Unity` was the graphical shell for the versions between Ubuntu 10 and 17. Ubuntu 18.04 swtiches back to GNOME 3 (with the native graphical shell).

Also look at here: [Comparison of X Window System desktop environments](https://en.wikipedia.org/wiki/Comparison_of_X_Window_System_desktop_environments#Desktop_comparison_information).

## GNOME

[`gsettings`](https://wiki.gnome.org/HowDoI/GSettings) is the tool to configure application settings on GNOME. `GSettings` can be seen as a database so it has the concept of `schemas`, `keys` and `values`.

Here is basic information about [`gconf`](https://en.wikipedia.org/wiki/GConf):

> GConf was a system used by the GNOME desktop environment for storing configuration settings for the desktop and applications. It is similar to the Windows Registry.
>
> It was deprecated as part of the GNOME 3 transition. Migration to its replacement, GSettings and dconf, is ongoing.

Here is basic information about [`dconf`](https://en.wikipedia.org/wiki/Dconf):

> dconf is a low-level configuration system and settings management. Its main purpose is to provide a back end to GSettings on platforms that don't already have configuration storage systems. It depends on GLib. It is part of GNOME 3 and is a replacement for GConf.

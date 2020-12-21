# SSH

## Overview

This document has the notes about SSH (assuming Ubuntu).

## Set up Debugging Environment

On the server side, do the following:

- Run `rsyslogd` to make sure syslog service is running because `auth` log is managed by it.
  - "This service itself is traditionally managed (started) by the init system (Upstart in your case). By default a container doesn't run an init system, meaning that you'd have to start rsyslog manually." See [this post](https://stackoverflow.com/a/22527114/630364).
  - `sudo service rsyslog restart`
- Modify `/etc/ssh/sshd_config`:
  - Change `LogLevel` to `VERBOSE`. See [this post](https://unix.stackexchange.com/a/15586/162971).
- If needed, stop the `ssh` service and run `sshd` directly:
  - `sudo service ssh stop`
  - `/usr/sbin/sshd -dd -D` where `-d` specifies the debug level and `-D` makes `sshd` run in the foreground.

## Find Key Fingerprint

Run `ssh-keygen -lf /path/to/ssh/key`, where:

- `-l`: List the key instead of creating it.
- `-f`: Use the given file.

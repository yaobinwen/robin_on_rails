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

## Remove a known (but out-dated) host key

`ssh-keygen -f "/home/ywen/.ssh/known_hosts" -R "10.0.0.10"`.

## Sign a certificate:

`ssh-keygen -s <path-to-CA-private-key> -I <identity> -n <principal> -V +1w -z <serial-no> <path-to-user-public-key>`
  - Example: `ssh-keygen -s ~/my_ca/private.key -I ywen-m4800 -n root-everywhere -V +1w -z 4800 "$HOME/.ssh/id_ecdsa.pub"`.

## Set up X11 forwarding

References:
- [1] [A note on X11-Forwarding in SSH](https://gist.github.com/adrianratnapala/1324845/b0d3df437ee5df5beef4bc1aed89af8c98a4246d)
- [2] [This answer](https://unix.stackexchange.com/a/317083/162971)

On the SSH server:
- Install `xauth`.
- In `/etc/ssh/sshd_config`, configure the following:

```
X11Forwarding yes
X11DisplayOffset 10
X11UseLocalhost no
```

- On the SSH client, in `~/.ssh/config`, configure the following:

```
Host *
  ForwardAgent yes
  ForwardX11 yes
```

- Run `ssh -X <target>`, then run `echo $DISPLAY` to confirm the output is not empty.

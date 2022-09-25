# DConf

## 1. DConf, GConf, and GSettings

See the question ["Gconf, Dconf, Gsettings and the relationship between them"](https://askubuntu.com/q/249887/514711). In sum:

- `GConf` is obsolete and has been replaced by DConf/GSettings. However, some applications may still use it.
- `DConf` is "simple key/value storage system that is heavily optimised for reading." [1] "Its main purpose is to provide a backend to GSettings on platforms that don't already have configuration storage systems." [2]
- `GSettings` is a development library used to read and write to a configuration store backend. On Linux, it uses `DConf`, but on Windows, it uses the Registry, and on OS X, it uses a native data store. (The `gsettings` command on the CLI uses this library.)

Application developers and end-users are recommended to use Gsettings, not Dconf directly.

References:
- [1] [ubuntu/+source/d-conf/README](https://git.launchpad.net/ubuntu/+source/d-conf/tree/README)
- [2] [GNOME: dconf](https://wiki.gnome.org/Projects/dconf)

## 2. DConf Database and GVariant

`dconf` database is a single file in binary format. The format is "GVariant Database file". Therefore, `dconf` uses data in the format of `GVairant` on the CLI.

`GVariant` is described [here](https://www.freedesktop.org/software/gstreamer-sdk/data/docs/latest/glib/glib-GVariant.html).

When calling the Ansible module `dconf`, the string value in `GVariant` format must be quoted properly. For example,

```yml
- name: Configure the power management.
  dconf:
    key: "/org/gnome/settings-daemon/plugins/power/sleep-inactive-battery-type"
    state: "present"
    value: "'suspend'"
```

If you wrote `value: "suspend"` without the single-quotation marks, that would fail with the error "error: 0-7:unknown keyword". The message is confusing and isn't helpful to identify the real problem. See the question [_dconf change a string key_](https://askubuntu.com/q/487206/514711) for an example.

## 3. DConf Database Files

A `dconf` profile is "a list of configuration databases that dconf consults to find the value for a key"[1]. A profile may look like the following:

```
user-db:user
system-db:local
system-db:site
```

> A "user-db" line specifies a user database. These databases are found in
>
> `$XDG_CONFIG_HOME/dconf/`. The name of the file to open in that directory is exactly as it
>
> is written in the profile.

`$XDG_CONFIG_HOME` is usually defined as `~/.config/`. So the user's `dconf` database file is at `~/.config/dconf/user`.

References:
- [1] [Ubuntu 18.04: dconf(7) manpage](http://manpages.ubuntu.com/manpages/bionic/man7/dconf.7.html)
- [2] [GNOME: dconf profiles](https://help.gnome.org/admin//system-admin-guide/3.8/dconf-profiles.html.en)
- [3] [How can I view the content of a backup of the dconf database file?](How can I view the content of a backup of the dconf database file?)
- [4] [XDG Base Directory Specification](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html)

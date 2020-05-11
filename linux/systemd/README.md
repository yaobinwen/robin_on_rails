# systemd

## Overview

[`systemd`](https://github.com/systemd/systemd) has two sites for documentation:

- [1] [systemd.io](https://systemd.io)
- [2] [systemd System and Service Manager](https://www.freedesktop.org/wiki/Software/systemd/)

I prefer [1], although [1] also links to some of the pages on [2].

Read these manuals first:

- [3] [systemd.syntax(7)](https://www.freedesktop.org/software/systemd/man/systemd.syntax.html): General syntax of systemd configuration files.
- [4] [systemd(1)](https://www.freedesktop.org/software/systemd/man/systemd.html): Description of `systemd`.
  - It's **highly recommended** to at least read the "Concepts" section.
    - This section explains the concept of `transaction` which is important to know.
- [5] [systemd.special(7)](https://www.freedesktop.org/software/systemd/man/systemd.special.html): Special systemd units.

The commonly used manuals may be the following ones. **Search the key words "preferred" and "recommended" for the best practices.**

- [6] [systemd.unit(5)](https://www.freedesktop.org/software/systemd/man/systemd.unit.html): Unit configuration.
- [7] [systemd.service(5)](https://www.freedesktop.org/software/systemd/man/systemd.service.html): Service unit configuration.
- [8] [systemd.target(5)](https://www.freedesktop.org/software/systemd/man/systemd.target.html): Target unit configuration.

Refer to the [Directives](https://www.freedesktop.org/software/systemd/man/systemd.directives.html) to see all the directives used in a `.service` definition file.

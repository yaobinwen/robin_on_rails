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

## systemd-journald

See the manual page: [systemd-journald](https://www.freedesktop.org/software/systemd/man/systemd-journald.service.html): "systemd-journald is a system service that collects and stores logging data. It creates and maintains structured, indexed journals based on logging information that is received from a variety of sources".

According to [_Why Journald?_](https://www.loggly.com/blog/why-journald/), `journald` was introduced to solve the problems of `syslog`. `syslog` typically writes log messages to plain text files without defining a particular structure, as a result:

> Finding information in large plain text files with lots of unrelated information can be difficult. Syslog implementations generally allow administrators to split up their files according to pre-defined topics, but they then end up with many smaller files and no easy way to correlate information between files.
>
> Additionally, the syslog protocol does not provide a means of separating messages by application-defined targets. For example, web servers can emit log messages per virtual host. Because syslog cannot deal with such meta information, the web servers generally write their own access logs so that the main system log is not flooded with web server status messages. This creates additional sources of possible log messages an admin has to keep in mind, with additional places where these messages are configured.
>
> Simple plain text files also require log rotation to prevent them from becoming too large. In log rotation, existing log files are renamed and compressed. Any programs that watch syslog messages for problems have to deal with this somehow. One common tool for this, the logcheck package, runs in a cron job and uses some heuristics to figure out when a log file was rotated and when to restart parsing a file. It is not unlikely that some log messages are lost in this process. Because of log rotation problems, some programs include the ability to directly notify admins of problems by email instead of using logging.

`journald` improves `syslog` by adding structure to log files and provides the tool [journalctl(1)](https://www.freedesktop.org/software/systemd/man/journalctl.html) to query the log files.

There is a big **but**: "Modern computing infrastructure utilizes many systems where it becomes impractical to read logs on individual machines."

> Centralized logging—where log messages from different systems are sent to a central log host and usually stored in a database—are increasingly becoming the standard way of logging. These log hosts address many of the same syslog issues that journald does, providing quick access to log messages by certain criteria, following new messages to generate reports, etc.

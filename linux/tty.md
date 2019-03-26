# TTY

## Overview

This document has the notes about the TTY-related knowledge. I wrote the notes as I explored this area and listed the topics in the order of "an earlier topic being the prerequisite of a later topic". I didn't list them in the same sequence as I learned about them. However, the references were appended as I found more and more of them. As a result, an earlier topic may refer to a reference that's at the bottom of the list.

## Terminal Emulator

As [4] explains:

> A terminal emulator ... is a program that emulates a video terminal within some other display architecture.

In the earlier days, a "video terminal" is a piece of hardware consisting of two parts:

- A display for output.
- A keyboard (maybe with a mouse) for input.

Typically, each "video terminal" served one user who ran his/her own programs on the remote machine (possibly a mainframe back then). This means that the programs running on the remote machine were grouped by the video terminals, or the users, that they came from.

So the terminal emulator tries to emulates a video terminal and its program isolation:

- Suppose the user is using a desktop system that has one physical display, one keyboard, and one mouse.
- Each terminal emulator is a program running with a graphical user interface on the host OS.
- This graphical user interface, or the window, emulates the display of a video terminal.
- The user can start multiple instances of the terminal emulator. Each instance emulates a video terminal.
- Of course, because these terminal emulators are the programs running on the host OS, their graphical user interfaces are all shown on the same physical display that the user can see.
- The user uses the keyboard and the mouse to provide input.
- The keyboard and mouse input is sent to the current foreground terminal emulator.
- Therefore, the keyboard, the mouse, and the terminal emulator's graphical user interface are the half-virtual-half-physical video terminal.
- The programs that are started from different terminal emulators, though possibly by the same user, are still considered as started by "different users" and put in different groups.

## Line Discipline

[1] says that, in a naive system, "the UART driver would then deliver the incoming bytes directly to some application process", but this approach would lack some important features, one of which is _line editing_. **My understanding** is: unless it is an editing tool, the usually processes whatever input is fed to it, and the processing may happen immediately when the input arrives. As a result, the user would not have a chance to fix the mistake if the input were sent to the application directly because when the user found the mistake, the input may already have been processed (and possibly resulted in garbage output).

The _line discipline_ works as a buffer (as [1] mentioned, in the _cooked_, or _canonical_, mode) and allows the user to correct the mistakes before sending it to the application. [2] lists three reasons for _line discipline_ to exist in the kernel:

- Make the serial communication fast because everything happens in the kernel space. The user space is not involved.
- Serve like a shared library to provide basic and consistent line editing functions to the applications.
- Make the I/O for a user application to a terminal the same as its I/O to a pipe or file.

**QUESTION**: What I haven't understood is why UART driver sending input to the application directly would result in the missing of session management.

[1] then defines `TTY`:

> Together, a particular triplet of UART driver, line discipline instance and TTY driver may be referred to as a _TTY device_, or sometimes just _TTY_. A user process can affect the behaviour of any TTY device by manipulating the corresponding device file under `/dev`.

[1] shows the architecture of a Linux desktop system that uses VGA display:

![Linux desktop system](http://www.linusakesson.net/programming/tty/case3.png)

It goes on to say:

> The console subsystem is somewhat rigid. Things get more flexible (and abstract) if we move the terminal emulation into userland. This is how `xterm(1)` and its clones work:
>
> ![User space terminal emulation](http://www.linusakesson.net/programming/tty/case4.png)

**QUESTION**: I don't quite understand why the desktop console subsystem is rigid, and how `xterm(1)`[3] makes it more flexible. More importantly, what does "flexible" mean?

As [5.1] explains:

> A _pseudo-terminal_ is a special interprocess communication channel that acts like a terminal. One end of the channel is called the master side or master pseudo-terminal device, the other side is called the slave side. Data written to the master side is received by the slave side as if it was the result of a user typing at an ordinary terminal, and data written to the slave side is sent to the master side as if it was written on an ordinary terminal.

## TTY Driver

The TTY driver structure is defined in [`include/linux/tty_driver.h`](https://github.com/torvalds/linux/blob/master/include/linux/tty_driver.h#L302). Another major structure used is [`tty_struct`](https://github.com/torvalds/linux/blob/master/include/linux/tty.h#L283). For more details, see [6].

## References

- [1] [The TTY demystified](http://www.linusakesson.net/programming/tty/)
- [2] [Why the TTY line discipline exists in the kernel](https://utcc.utoronto.ca/~cks/space/blog/unix/TTYLineDisciplineWhy)
- [3] [xterm(1)](https://www.x.org/archive/X11R6.7.0/doc/xterm.1.html)
- [4] [Terminal emulator](https://en.wikipedia.org/wiki/Terminal_emulator)
- [5] Pseudo-terminal
  - [5.1] [17.9 Pseudo-Terminals](https://www.gnu.org/software/libc/manual/html_node/Pseudo_002dTerminals.html)
  - [5.2] [pty(7) - Linux man page](https://linux.die.net/man/7/pty)
  - [5.3] [pts(4) - Linux man page](https://linux.die.net/man/4/pts)
- [6] ["Chapter 18. TTY Drivers" of "Linux Device Drivers, 3rd Edition"](https://www.oreilly.com/library/view/linux-device-drivers/0596005903/ch18.html)
  - [Linux Device Drivers, 3rd Edition](https://www.amazon.com/Linux-Device-Drivers-Jonathan-Corbet/dp/0596005903)

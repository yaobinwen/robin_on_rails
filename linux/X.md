# X Window System

## The "DISPLAY" Environment Variable

[1] explains the environment variable well and concisely, so I'll quote the (almost) entire article here.

A display consists (simplified) of:
- a keyboard,
- a mouse
- and a screen.

(**QUESTION**: But why does a display include a keyboard and a mouse?)

A display is managed by a server program, known as an X server. The server serves displaying capabilities to other programs that connect to it.
The remote server knows where it have to redirect the X network traffic via the definition of the DISPLAY environment variable which generally points to an X Display server located on your local computer.

The value of the display environment variable is `hostname:displaynumber.screennumber` where:
- `hostname` is the name of the computer where the X server runs. An omitted hostname means the localhost.
- `displaynumber(D)` is a sequence number (usually 0). It can be varied if there are multiple displays connected to one computer. "The display number must always be given in a display name."[2]
- `screennumber(S)` is the screen number. A display can actually have multiple screens. Usually there's only one screen though where 0 is the default.

Example of values:
- `localhost:4`
- `google.com:0`
- `:0.0`

`hostname:D.S` means screen `S` on display `D` of host `hostname`; the X server for this display is listening at TCP port 6000+D. See Note [1].

`host/unix:D.S` means screen `S` on display `D` of host `host`; the X server for this display is listening at UNIX domain socket `/tmp/.X11-unix/XD`(so it's only reachable from host). See Note [2].

`:D.S` is equivalent to `host/unix:D.S`, where host is the local hostname.

`:0.0` means that we are talking about the first screen attached to your first display in your local host.

Note [1]:

According to [2], "X servers generally support the following types of connections:"
- `local`: The hostname part of the display name should be the empty string.
- `TCP/IP`: The hostname part of the display name should be the server machine's IP address name. Full Internet names, abbreviated names, and IP addresses are all allowed.
- `DECnet`: See [2] for more info.

Therefore, in the case that `hostname` is not empty, X server uses a TCP/IP connection.

The port 6000 is mentioned in [3], Appendix B, section "Connection Setup" (Page 114):

> For TCP connections, displays on a given host are numbered starting from 0, and the server for display N listens and accepts connections on port 6000 + N.

The port 6000 is also mentioned in [4]. See the registration table at the bottom.

Note [2]:

The `D` in `/tmp/.X11-unix/XD` is a number. On my current work computer, it is `/tmp/.X11-unix/X0=` where `DISPLAY` is `:0`.

## X Window System Architecture

X Window system is a `client-server` architecture. See [5].

![Architecture](https://upload.wikimedia.org/wikipedia/commons/thumb/0/03/X_client_server_example.svg/440px-X_client_server_example.svg.png)

In a more widely seen model, the user who sits in front of a keyboard, a mouse, and a display device is usually on the "client" side, and he/she uses the keyboard and the mouse to provide input which is sent over the network to the "server" side which computes and sends the result back to the client side to be displayed on the display device.

However, in X, this is reversed: The machine that the user uses is the `server`, and the applications that produce graphical output, regardless running locally or remotely, are the `clients`, hence the architectural view as shown above.

The `server` has two responsibilities:

- Take the input from the keyboard and the mouse and send it to the clients.
- Take the output from the clients and render it to the display device.

[5] also mentions these terms:

- `window manager`
- `widget library` or `toolkit`
- `GTK` and `QT`
- `desktop environment`
- `KDE` and `GNOME`

## References

- [1] [What is the $DISPLAY environment variable?](https://askubuntu.com/a/432257/514711)
- [2] [X(7) man page](https://linux.die.net/man/7/x)
  - Or run `man x 7` in a terminal to view the man page.
- [3] [X Window System Protocol, X Version 11, Release 6.7](https://www.x.org/docs/XProtocol/proto.pdf)
- [4] [Service Name and Transport Protocol Port Number Registry](https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml?search=x11)
- [5] [X Window System Architecture Overview HOWTO](https://www.tldp.org/HOWTO/XWindow-Overview-HOWTO/index.html)

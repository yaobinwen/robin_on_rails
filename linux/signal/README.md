# README

[Termination Signals (from the most to the least polite)](https://www.gnu.org/software/libc/manual/html_node/Termination-Signals.html):

- SIGTERM
- SIGINT
- SIGQUIT
- SIGKILL
- SIGHUP

---

How Signals Are Delivered to Forked Processes

[Here is the question](https://unix.stackexchange.com/q/176235/162971):

> I program that I wrote in C fork()'s off a child process. Neither process will terminate. If I launch the program from the command line and press control-c which process(es) will receive the interrupt signal?

[The answer](https://unix.stackexchange.com/a/176242/162971) provides a sample code for demo:

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
void parent_trap(int sig) {fprintf(stderr, "They got back together!\n");}
void child_trap(int sig) {fprintf(stderr, "Caught signal in CHILD.\n");}
int main(int argc, char **argv) {
    if (!fork()) {
        signal(SIGINT, &child_trap);
        sleep(1000);
        exit(0);
    }
    signal(SIGINT, &parent_trap);
    sleep(1000);
    return 0;
}
```

And the result is:

```
$ gcc test.c
$ ./a.out
^CCaught signal in CHILD.
They got back together!
```

**Interrupt signals generated in the terminal are delivered to the active process group, which here includes both parent and child.**

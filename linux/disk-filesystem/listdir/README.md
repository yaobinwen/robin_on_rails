# README

## Overview

This is a summary of the article [1].

The `listdir.c` is the code that lists the directory that has many files. Run `gcc -o listdir listdir.c` to compile it.

## Notes

`ls`, `os.listdir` and `find` use the `readdir()`[2] which "only reads 32K of directory entries at a time" [1]. This makes the reading of a large directory slow.

`getdents()`[3] is the rescue. The man page provides a sample implementation:

```c
#define _GNU_SOURCE
#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct linux_dirent {
    long           d_ino;
    off_t          d_off;
    unsigned short d_reclen;
    char           d_name[];
};

#define BUF_SIZE 1024

int
main(int argc, char *argv[])
{
    int fd, nread;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    int bpos;
    char d_type;

    fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | O_DIRECTORY);
    if (fd == -1)
        handle_error("open");

    for ( ; ; ) {
        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        if (nread == -1)
            handle_error("getdents");

        if (nread == 0)
            break;

        printf("--------------- nread=%d ---------------\n", nread);
        printf("inode#    file type  d_reclen  d_off   d_name\n");
        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            printf("%8ld  ", d->d_ino);
            d_type = *(buf + bpos + d->d_reclen - 1);
            printf("%-10s ", (d_type == DT_REG) ?  "regular" :
                            (d_type == DT_DIR) ?  "directory" :
                            (d_type == DT_FIFO) ? "FIFO" :
                            (d_type == DT_SOCK) ? "socket" :
                            (d_type == DT_LNK) ?  "symlink" :
                            (d_type == DT_BLK) ?  "block dev" :
                            (d_type == DT_CHR) ?  "char dev" : "???");
            printf("%4d %10lld  %s\n", d->d_reclen,
                    (long long) d->d_off, d->d_name);
            bpos += d->d_reclen;
        }
    }

    exit(EXIT_SUCCESS);
}
```

We want to make two changes:

- Increase the `BUF_SIZE` to a larger value in order to retrieve all the information shortly.
- Skip the entries whose `inode` is `0`. (Why?)

Takeaways from the article:

- It is possible to list a directory with 8 million files in it.
- `strace`[4] is your friend.
- Don't be afraid to compile code and modify it (hell, simple C compiles so fast it could be interpreted)
- There is no good reason to have 8 million files in a directory.

## References

- [1] [You can list a directory containing 8 million files! But not with ls.](http://be-n.com/spw/you-can-list-a-million-files-in-a-directory-but-not-with-ls.html)
- [2] [`readdir()`](http://man7.org/linux/man-pages/man3/readdir.3.html)
- [3] [`getdents()`](http://man7.org/linux/man-pages/man2/getdents.2.html)
- [4] [`strace`: trace system calls and signals](http://man7.org/linux/man-pages/man1/strace.1.html)

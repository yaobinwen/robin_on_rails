# The Infamous Bumblebee Bug

## Overview

I keep this in record not to laugh at the programmers who made the mistake. I keep this as a reminder to myself that how terrifying using `root` can be and how much responsibility us programmers have in the current society: with IT technologies widely used, the programmers must be very careful to avoid (or at least find) such mistakes before they create a wide impact to the end users.

[`Bumblebee-bug.zip`](./Bumblebee-bug.zip) is the `.zip` file that contains the HTML file and the corresponding resources (mostly images). Unzip it and open the HTML file in a browser to read.

## Recap

The (now abandoned) [`Bumblebee` project](https://github.com/MrMEEE/bumblebee-Old-and-abbandoned) received three bug reports regarding the error "/usr/local/bin/': No such file or directory":
- [(2011-05-23) #121: Problems with Alienware M14x and Ubuntu 11.04](https://github.com/MrMEEE/bumblebee-Old-and-abbandoned/issues/121)
- [(2011-05-23) #122: Dell XPS 15 (L502X), Ubuntu 11.04, cp: cannot create regular file `/usr/local/bin/' and other errors during installation](https://github.com/MrMEEE/bumblebee-Old-and-abbandoned/issues/122)
- [(2011-05-24) #123: install script does rm -rf /usr for ubuntu](https://github.com/MrMEEE/bumblebee-Old-and-abbandoned/issues/123)

On 2011-05-24, the commit "GIANT BUG... causing /usr to be deleted... so sorry.... issue #123, issue #122, issue #121" [1] was pushed in which the author removed the blank space :

```shell
-   rm -rf /usr /lib/nvidia-current/xorg/xorg
+   rm -rf /usr/lib/nvidia-current/xorg/xorg
```

## Another Bug by Apple

[The Hacker News thread "My favorite all time bug was the infamous "Bumblebee" commit of 2011"](https://news.ycombinator.com/item?id=23054506) mentions a bug that Apple did back in 2001:

> Though my continuing "favorite" (more amusing in hindsight) similar one was when Apple did that, back in like 2001 with, iTunes 1? iTunes 2? Or maybe it was one of the Mac OS X 10.1 updates ...
>
> At any rate, lots of people at Apple back then brand new to Unix, NeXT was still integrating, everything was still coming together. And they made one of the absolute most classic Unix newbie whoops moments: they wanted to clean up old versions of iTunes, so the used rm -rf... without quoting the path. It had this IIRC:
>
>     rm -rf $2Applications/iTunes.app 2
>
> with $2 as the path. But of course classic Mac users were used to having spaces in drive names and folders and so on. If you only had the startup drive no problem. But if you'd partitioned or had an external drive and it had a space in the name, ie "Disk 1", then that'd become rm -rf Disk 1/Applications/iTunes.app 2 and you were off to the races.

It provides the link [3] for the discussion about this issue. I saved the discussion in [`iTunes-2.0.zip`](./iTunes-2.0.zip).

## References

- [1] [The original GitHub commit](https://github.com/MrMEEE/bumblebee-Old-and-abbandoned/commit/a047be85247755cdbe0acce6f1dafc8beb84f2ac): The link may take time to load and may result in 50X error. Refresh a few times if that happens.
- [2] If [1] doesn't load, try [this backup page](https://www.netmeister.org/blog/MrMEEE/backup.html).
- [3] [iTunes 2.0 Installer Deletes Hard Drives](https://apple.slashdot.org/story/01/11/04/0412209/itunes-20-installer-deletes-hard-drives)

# Disk Data & Health

The tools that are mentioned here are all on Ubuntu 18.04.

The tool [`dd(1)`](https://manpages.ubuntu.com/manpages/bionic/en/man1/dd.1.html) "convert and copy a file". One use of it is to write random data onto the disk. For example, the following command writes random data to the entire device `/dev/sdb`:

```
sudo dd if=/dev/urandom of=/dev/sdb bs=4K
```

The tool [`badblocks(8)`](https://manpages.ubuntu.com/manpages/bionic/man8/badblocks.8.html) "search a device for bad blocks" by performing various types of tests. Note that `badblocks` is "part of the [`e2fsprogs`](http://e2fsprogs.sourceforge.net/) package" which is a set of Ext2/3/4 filesystem utilities.

However, note that, as mentioned in [this answer](https://askubuntu.com/a/241951/514711):

> A **bad sector** on a drive is a sign of permanent damage to the drive. Unless you have reason to believe that your drive marked these sectors as bad incorrectly, you cannot "fix" them.
>
> It means that a part of your drive is damaged to the extent that it can no longer reliably be read from and/or written to.

A related tool is [`fsck(8)`](https://manpages.ubuntu.com/manpages/bionic/man8/fsck.8.html) which "check and repair a Linux filesystem".

The package [`smartmontools`](https://www.smartmontools.org/) contains utilities to check the physical condition of [`S.M.A.R.T.`](https://en.wikipedia.org/wiki/S.M.A.R.T.) disks:

> **S.M.A.R.T.** (**Self-Monitoring, Analysis and Reporting Technology**; often written as **SMART**) is a monitoring system included in computer hard disk drives (HDDs), solid-state drives (SSDs), and eMMC drives. Its primary function is to detect and report various indicators of drive reliability with the intent of anticipating imminent hardware failures. 

[This answer](https://askubuntu.com/a/490549/514711) provides a quick and simple tutorial of how to use `smartctl`.

# Disk and Filesystem

## Disks and Partitions

What is a disk partition? As explained in [1]:

> Hard disks, USB drives, SD cards — anything with storage space must be partitioned. An unpartitioned drive can’t be used until it contains at least one partition, but a drive can contain multiple partitions. ... To actually set up a file system and save any files to the drive, the drive needs a partition.

[1] further says:

> ... all storage devices are just treated as a mass of unallocated, free space when they contain no partitions. ... Partitions are necessary because you can’t just start writing files to a blank drive. You must first create at least one container with a file system. We call this container a partition.

It looks like the need of a partition is due to the need of a file system; the need of a file system is due to the need of structure, organization, storage, and retrieval of files. The partition is a like a "container" of a particular file system. With the file system installed, users can then write and read files to and from that part of the disk, i.e. the partition.

With that said, a disk, or more generally, a storage device, may have several partitions and each partition has a different file system.

What are _primary_, _extended_, and _logical_ partitions?

[1] says:

> A disk with a traditional partition table can only have up to four partitions.

Therefore, without any special techniques, a partition is usually a _primary_ partition.

The following paragraph of [1] explains how the _extended_ partition helps creating more partitions:

> However, let’s say you want six partitions on a single drive. You’d have to create three primary partitions as well as an extended partition. The extended partition effectively functions as a container that allows you to create a larger amount of logical partitions. So, if you needed six partitions, you’d create three primary partitions, an extended partition, and then three logical partitions inside the extended partition. You could also just create a single primary partition, an extended partition, and five logical partitions — you just can’t have more than four primary partitions at a time.

## Ext Filesystems

`Ext3`:

> The only addition to the EXT filesystem was the **journal**, which records in advance the changes that will be performed to the filesystem. ... Instead of writing data to the disk's data areas directly, as in previous versions, the journal in EXT3 writes file data, along with its metadata, to a specified area on the disk. Once the data is safely on the hard drive, it can be merged in or appended to the target file with almost zero chance of losing data. As this data is committed to the data area of the disk, the journal is updated so that the filesystem will remain in a consistent state in the event of a system failure before all the data in the journal is committed. On the next boot, the filesystem will be checked for inconsistencies, and data remaining in the journal will then be committed to the data areas of the disk to complete the updates to the target file.

`Ext4`:

> The EXT4 filesystem primarily improves performance, reliability, and capacity.

## References

- [1] [Beginner Geek: Hard Disk Partitions Explained](https://www.howtogeek.com/184659/beginner-geek-hard-disk-partitions-explained/)
- [2] [Understanding Linux filesystems: ext4 and beyond](https://opensource.com/article/18/4/ext4-filesystem)
- [3] [Anatomy of Linux journaling file systems: Journaling today and tomorrow](https://www.ibm.com/developerworks/library/l-journaling-filesystems/l-journaling-filesystems-pdf.pdf)
- [4] [A MINIMUM COMPLETE TUTORIAL OF LINUX EXT4 FILE SYSTEM](https://metebalci.com/blog/a-minimum-complete-tutorial-of-linux-ext4-file-system/)
- [5] ["safe" ext4 configuration for systems running unattended](https://serverfault.com/q/356507/125167)
- [6] [Is ext4 filesystem safe?](https://unix.stackexchange.com/q/474496/162971)

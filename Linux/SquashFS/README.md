# SquashFS

Refer to [1][SquashFS HOWTO](https://tldp.org/HOWTO/SquashFS-HOWTO/index.html).

Basic steps:
- Create the file: `mksquashfs /some/dir dir.sqsh`
  - The created `dir.sqsh` is displayed as of 4.0KB only, but one can store much larger files inside it (I stored a 100MB file in it for testing).
  - The created `dir.sqsh` can be copied to other locations or machines.
- Mount it: `sudo mount -t squashfs -o loop dir.sqsh /mnt/dir`
- Unmount it: `sudo umount /mnt/dir`

`SquashFS` can be used to compress a large file system into a smaller, read-only one. See `4.2.2. Example 2` in [1].

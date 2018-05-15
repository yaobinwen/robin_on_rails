# README

## Search Paths of Libraries

According to [`ld.so(8)` manual](http://man7.org/linux/man-pages/man8/ld.so.8.html), the precedence of library search paths is as follows:

- `DT_RPATH`(deprecated)
- The environment variable `LD_LIBRARY_PATH`
- `DT_RUNPATH` dynamic section attribute of the binary, if present.
- Cache file `/etc/ld.so.cache`
- Default path `/lib`.
- `/usr/lib`

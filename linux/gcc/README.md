# README

## Search Paths of Libraries

According to [`ld.so(8)` manual](http://man7.org/linux/man-pages/man8/ld.so.8.html), the precedence of library search paths is as follows:

- `DT_RPATH`(deprecated)
- The environment variable `LD_LIBRARY_PATH`
- `DT_RUNPATH` dynamic section attribute of the binary, if present.
- Cache file `/etc/ld.so.cache`
- Default path `/lib`.
- `/usr/lib`

Here is how to print `gcc` or `g++` search directories: Run `g++ -print-search-dirs` in a terminal. The sample output is:

```text
libraries:
- /usr/lib/gcc/x86_64-linux-gnu/4.9/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../../x86_64-linux-gnu/lib/x86_64-linux-gnu/4.9/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../../x86_64-linux-gnu/lib/x86_64-linux-gnu/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../../x86_64-linux-gnu/lib/../lib/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../x86_64-linux-gnu/4.9/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../x86_64-linux-gnu/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../../lib/
- /lib/x86_64-linux-gnu/4.9/
- /lib/x86_64-linux-gnu/
- /lib/../lib/
- /usr/lib/x86_64-linux-gnu/4.9/
- /usr/lib/x86_64-linux-gnu/
- /usr/lib/../lib/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../../x86_64-linux-gnu/lib/
- /usr/lib/gcc/x86_64-linux-gnu/4.9/../../../
- /lib/
- /usr/lib/
```

You can also look at the search directories of `ld` by running `ld --verbose | grep SEARCH` (specified in `/etc/ld.so.conf.d`):

```text
- SEARCH_DIR("/usr/x86_64-linux-gnu/lib64");
- SEARCH_DIR("=/usr/local/lib/x86_64-linux-gnu");
- SEARCH_DIR("=/usr/local/lib64");
- SEARCH_DIR("=/lib/x86_64-linux-gnu");
- SEARCH_DIR("=/lib64");
- SEARCH_DIR("=/usr/lib/x86_64-linux-gnu");
- SEARCH_DIR("=/usr/lib64");
- SEARCH_DIR("=/usr/local/lib");
- SEARCH_DIR("=/lib");
- SEARCH_DIR("=/usr/lib");
```

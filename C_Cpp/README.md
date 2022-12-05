# C/C++

## Source code

The GNU C Library (glibc):
- [Homepage](https://www.gnu.org/software/libc/libc.html)
- Git checkout: `git clone https://sourceware.org/git/glibc.git`

GCC, the GNU Compiler Collection:
- [Homepage](https://gcc.gnu.org/)
- [C++ Standards Support in GCC](https://gcc.gnu.org/projects/cxx-status.html)
- [GCC: Anonymous read-only Git access](https://gcc.gnu.org/git.html)
  - Git checkout: `git clone https://gcc.gnu.org/git/gcc.git`
    - The C++ library is under the folder `libstdc++-v3`. For example, `vector`'s header file is `libstdc++-v3/include/std/vector` which uses the implementation code in `libstdc++-v3/include/bits/stl_vector.h`.

## C `fopen`

The source code is in `glibc`.

`FILE` is defined in `libio/bits/types/FILE.h`:

```c
struct _IO_FILE;

/* The opaque type of streams.  This is the definition used elsewhere.  */
typedef struct _IO_FILE FILE;
```

`_IO_FILE`, together with many other supportive types, is defined in `libio/bits/types/struct_FILE.h`:

```c
/* The tag name of this struct is _IO_FILE to preserve historic
   C++ mangled names for functions taking FILE* arguments.
   That name should not be used in new code.  */
struct _IO_FILE
{
  int _flags;		/* High-order word is _IO_MAGIC; rest is flags. */

  /* The following pointers correspond to the C++ streambuf protocol. */
  char *_IO_read_ptr;	/* Current read pointer */
  char *_IO_read_end;	/* End of get area. */
  char *_IO_read_base;	/* Start of putback+get area. */
  char *_IO_write_base;	/* Start of put area. */
  char *_IO_write_ptr;	/* Current put pointer. */
  char *_IO_write_end;	/* End of put area. */
  char *_IO_buf_base;	/* Start of reserve area. */
  char *_IO_buf_end;	/* End of reserve area. */

  /* The following fields are used to support backing up and undo. */
  char *_IO_save_base; /* Pointer to start of non-current get area. */
  char *_IO_backup_base;  /* Pointer to first valid character of backup area */
  char *_IO_save_end; /* Pointer to end of non-current get area. */

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2;
  __off_t _old_offset; /* This used to be _offset but it's too small.  */

  /* 1+column number of pbase(); 0 is unknown. */
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;
#ifdef _IO_USE_OLD_IO_FILE
};

struct _IO_FILE_complete
{
  struct _IO_FILE _file;
#endif
  __off64_t _offset;
  /* Wide character stream stuff.  */
  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  size_t __pad5;
  int _mode;
  /* Make sure we don't get into trouble again.  */
  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];
};
```

Note the use of `#ifdef _IO_USE_OLD_IO_FILE`: If `_IO_USE_OLD_IO_FILE` is defined, the whole `struct` is broken into two `struct`s; otherwise, `_IO_FILE` also includes the fields such as `_offset`, `__pad5`, `_mode`, etc.

`fopen` is declared in `include/stdio.h` as a macro that's defined to refer to `_IO_new_fopen`:

```c
#  if IS_IN (libc)
extern FILE *_IO_new_fopen (const char*, const char*);
#   define fopen(fname, mode) _IO_new_fopen (fname, mode)
extern FILE *_IO_new_fdopen (int, const char*);
#   define fdopen(fd, mode) _IO_new_fdopen (fd, mode)
extern int _IO_new_fclose (FILE*);
#   define fclose(fp) _IO_new_fclose (fp)
extern int _IO_fputs (const char*, FILE*);
libc_hidden_proto (_IO_fputs)
/* The compiler may optimize calls to fprintf into calls to fputs.
   Use libc_hidden_proto to ensure that those calls, not redirected by
   the fputs macro, also do not go through the PLT.  */
libc_hidden_proto (fputs)
#   define fputs(str, fp) _IO_fputs (str, fp)
extern int _IO_new_fsetpos (FILE *, const __fpos_t *);
#   define fsetpos(fp, posp) _IO_new_fsetpos (fp, posp)
extern int _IO_new_fgetpos (FILE *, __fpos_t *);
#   define fgetpos(fp, posp) _IO_new_fgetpos (fp, posp)
#  endif
```

`_IO_new_fopen` is defined in `libio/iofopen.c`:

```c
FILE *
_IO_new_fopen (const char *filename, const char *mode)
{
  return __fopen_internal (filename, mode, 1);
}
```

`__fopen_internal` is defined in `libio/iofopen.c`:

```c
FILE *
__fopen_internal (const char *filename, const char *mode, int is32)
{
  struct locked_FILE
  {
    struct _IO_FILE_plus fp;
#ifdef _IO_MTSAFE_IO
    _IO_lock_t lock;
#endif
    struct _IO_wide_data wd;
  } *new_f = (struct locked_FILE *) malloc (sizeof (struct locked_FILE));

  if (new_f == NULL)
    return NULL;
#ifdef _IO_MTSAFE_IO
  new_f->fp.file._lock = &new_f->lock;
#endif
  _IO_no_init (&new_f->fp.file, 0, 0, &new_f->wd, &_IO_wfile_jumps);
  _IO_JUMPS (&new_f->fp) = &_IO_file_jumps;
  _IO_new_file_init_internal (&new_f->fp);
  if (_IO_file_fopen ((FILE *) new_f, filename, mode, is32) != NULL)
    return __fopen_maybe_mmap (&new_f->fp.file);

  _IO_un_link (&new_f->fp);
  free (new_f);
  return NULL;
}
```

The other functions that `__fopen_internal` calls are:
- `_IO_no_init`: in `libio/genops.c`.
- `_IO_JUMPS`: in `libio/libioP.h`. It is a macro.
- `_IO_file_fopen`: I didn't find it, but possibly it's the same as `_IO_old_file_fopen` that's defined in `libio/oldfileops.c`.
  - `_IO_old_file_fopen` eventually calls `__open` to open the file. `__open` is defined as a macro in `sysdeps/posix/tempname.c`. This seems to suggest that the POSIX system call [`open`](https://pubs.opengroup.org/onlinepubs/007904875/functions/open.html) is called here.
- `__fopen_maybe_mmap`: in `libio/iofopen.c`.
- `_IO_un_link`: in `libio/genops.c`.

## Links

- [ISO C++](https://isocpp.org/)
- [Working Draft, Standard for Programming Language C++](https://eel.is/c++draft/)
- [JTC1/SC22/WG21 - The C++ Standards Committee - ISOCPP](https://www.open-std.org/jtc1/sc22/wg21/)
- [The Definitive C++ Book Guide and List](https://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
- [Using C++11's Smart Pointers](http://umich.edu/~eecs381/handouts/C++11_smart_ptrs.pdf)
- How `switch` is implemented: [From Switch Statement Down to Machine Code](http://lazarenko.me/switch/)
- [C++ Compiler Explorer](https://godbolt.org/) shows you how the C++ source code is generated to machine code.
- [Duff's device](https://en.wikipedia.org/wiki/Duff%27s_device) is "a way of manually implementing [loop unrolling](https://en.wikipedia.org/wiki/Loop_unrolling) by interleaving two syntactic constructs of C: the do-while loop and a switch statement."
- [再再论指针](https://blog.csdn.net/megaboy/article/details/482783): This is a series of articles I read to learn the C pointers. They are old.
- [Coliru](https://coliru.stacked-crooked.com/): An online tool to write C++ code for quick verification of something.
- [How to read in a file in C++](http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html)
- Rvalue references:
  - [A Brief Introduction to Rvalue References](https://www.artima.com/cppsource/rvalue.html)
  - [C++ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_01.html)
- Default initialization of POD types: See [this answer](https://stackoverflow.com/a/15212447/630364). Note it uses C++ 03 standard.

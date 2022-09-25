/* Malloc implementation for multiple threads without lock contention.
   Copyright (C) 1996-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Wolfram Gloger <wg@malloc.de>
   and Doug Lea <dl@cs.oswego.edu>, 2001.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <http://www.gnu.org/licenses/>.  */

/*
  This is a version (aka ptmalloc2) of malloc/free/realloc written by
  Doug Lea and adapted to multiple threads/arenas by Wolfram Gloger.

  There have been substantial changes made after the integration into
  glibc in all parts of the code.  Do not look for much commonality
  with the ptmalloc2 version.

* Version ptmalloc2-20011215
  based on:
  VERSION 2.7.0 Sun Mar 11 14:14:06 2001  Doug Lea  (dl at gee)

* Quickstart

  In order to compile this implementation, a Makefile is provided with
  the ptmalloc2 distribution, which has pre-defined targets for some
  popular systems (e.g. "make posix" for Posix threads).  All that is
  typically required with regard to compiler flags is the selection of
  the thread package via defining one out of USE_PTHREADS, USE_THR or
  USE_SPROC.  Check the thread-m.h file for what effects this has.
  Many/most systems will additionally require USE_TSD_DATA_HACK to be
  defined, so this is the default for "make posix".

* Why use this malloc?

  This is not the fastest, most space-conserving, most portable, or
  most tunable malloc ever written. However it is among the fastest
  while also being among the most space-conserving, portable and tunable.
  Consistent balance across these factors results in a good general-purpose
  allocator for malloc-intensive programs.

  The main properties of the algorithms are:
  * For large (>= 512 bytes) requests, it is a pure best-fit allocator,
    with ties normally decided via FIFO (i.e. least recently used).
  * For small (<= 64 bytes by default) requests, it is a caching
    allocator, that maintains pools of quickly recycled chunks.
  * In between, and for combinations of large and small requests, it does
    the best it can trying to meet both goals at once.
  * For very large requests (>= 128KB by default), it relies on system
    memory mapping facilities, if supported.

  For a longer but slightly out of date high-level description, see
     http://gee.cs.oswego.edu/dl/html/malloc.html

  You may already by default be using a C library containing a malloc
  that is  based on some version of this malloc (for example in
  linux). You might still want to use the one in this file in order to
  customize settings or to avoid overheads associated with library
  versions.

* Contents, described in more detail in "description of public routines" below.

  Standard (ANSI/SVID/...)  functions:
    malloc(size_t n);
    calloc(size_t n_elements, size_t element_size);
    free(void* p);
    realloc(void* p, size_t n);
    memalign(size_t alignment, size_t n);
    valloc(size_t n);
    mallinfo()
    mallopt(int parameter_number, int parameter_value)

  Additional functions:
    independent_calloc(size_t n_elements, size_t size, void* chunks[]);
    independent_comalloc(size_t n_elements, size_t sizes[], void* chunks[]);
    pvalloc(size_t n);
    cfree(void* p);
    malloc_trim(size_t pad);
    malloc_usable_size(void* p);
    malloc_stats();

* Vital statistics:

  Supported pointer representation:       4 or 8 bytes
  Supported size_t  representation:       4 or 8 bytes
       Note that size_t is allowed to be 4 bytes even if pointers are 8.
       You can adjust this by defining INTERNAL_SIZE_T

  Alignment:                              2 * sizeof(size_t) (default)
       (i.e., 8 byte alignment with 4byte size_t). This suffices for
       nearly all current machines and C compilers. However, you can
       define MALLOC_ALIGNMENT to be wider than this if necessary.

  Minimum overhead per allocated chunk:   4 or 8 bytes
       Each malloced chunk has a hidden word of overhead holding size
       and status information.

  Minimum allocated size: 4-byte ptrs:  16 bytes    (including 4 overhead)
			  8-byte ptrs:  24/32 bytes (including, 4/8 overhead)

       When a chunk is freed, 12 (for 4byte ptrs) or 20 (for 8 byte
       ptrs but 4 byte size) or 24 (for 8/8) additional bytes are
       needed; 4 (8) for a trailing size field and 8 (16) bytes for
       free list pointers. Thus, the minimum allocatable size is
       16/24/32 bytes.

       Even a request for zero bytes (i.e., malloc(0)) returns a
       pointer to something of the minimum allocatable size.

       The maximum overhead wastage (i.e., number of extra bytes
       allocated than were requested in malloc) is less than or equal
       to the minimum size, except for requests >= mmap_threshold that
       are serviced via mmap(), where the worst case wastage is 2 *
       sizeof(size_t) bytes plus the remainder from a system page (the
       minimal mmap unit); typically 4096 or 8192 bytes.

  Maximum allocated size:  4-byte size_t: 2^32 minus about two pages
			   8-byte size_t: 2^64 minus about two pages

       It is assumed that (possibly signed) size_t values suffice to
       represent chunk sizes. `Possibly signed' is due to the fact
       that `size_t' may be defined on a system as either a signed or
       an unsigned type. The ISO C standard says that it must be
       unsigned, but a few systems are known not to adhere to this.
       Additionally, even when size_t is unsigned, sbrk (which is by
       default used to obtain memory from system) accepts signed
       arguments, and may not be able to handle size_t-wide arguments
       with negative sign bit.  Generally, values that would
       appear as negative after accounting for overhead and alignment
       are supported only via mmap(), which does not have this
       limitation.

       Requests for sizes outside the allowed range will perform an optional
       failure action and then return null. (Requests may also
       also fail because a system is out of memory.)

  Thread-safety: thread-safe

  Compliance: I believe it is compliant with the 1997 Single Unix Specification
       Also SVID/XPG, ANSI C, and probably others as well.

* Synopsis of compile-time options:

    People have reported using previous versions of this malloc on all
    versions of Unix, sometimes by tweaking some of the defines
    below. It has been tested most extensively on Solaris and Linux.
    People also report using it in stand-alone embedded systems.

    The implementation is in straight, hand-tuned ANSI C.  It is not
    at all modular. (Sorry!)  It uses a lot of macros.  To be at all
    usable, this code should be compiled using an optimizing compiler
    (for example gcc -O3) that can simplify expressions and control
    paths. (FAQ: some macros import variables as arguments rather than
    declare locals because people reported that some debuggers
    otherwise get confused.)

    OPTION                     DEFAULT VALUE

    Compilation Environment options:

    HAVE_MREMAP                0

    Changing default word sizes:

    INTERNAL_SIZE_T            size_t
    MALLOC_ALIGNMENT           MAX (2 * sizeof(INTERNAL_SIZE_T),
				    __alignof__ (long double))

    Configuration and functionality options:

    USE_PUBLIC_MALLOC_WRAPPERS NOT defined
    USE_MALLOC_LOCK            NOT defined
    MALLOC_DEBUG               NOT defined
    REALLOC_ZERO_BYTES_FREES   1
    TRIM_FASTBINS              0

    Options for customizing MORECORE:

    MORECORE                   sbrk
    MORECORE_FAILURE           -1
    MORECORE_CONTIGUOUS        1
    MORECORE_CANNOT_TRIM       NOT defined
    MORECORE_CLEARS            1
    MMAP_AS_MORECORE_SIZE      (1024 * 1024)

    Tuning options that are also dynamically changeable via mallopt:

    DEFAULT_MXFAST             64 (for 32bit), 128 (for 64bit)
    DEFAULT_TRIM_THRESHOLD     128 * 1024
    DEFAULT_TOP_PAD            0
    DEFAULT_MMAP_THRESHOLD     128 * 1024
    DEFAULT_MMAP_MAX           65536

    There are several other #defined constants and macros that you
    probably don't want to touch unless you are extending or adapting malloc.  */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for getenv(), abort() */
#include <unistd.h>   /* for __libc_enable_secure */

// #include <malloc-machine.h>
// #include <malloc-sysdep.h>

// #include <atomic.h>
// #include <_itoa.h>
#include <bits/wordsize.h>
#include <sys/sysinfo.h>

// #include <ldsodefs.h>

#include <unistd.h>
#include <stdio.h>    /* needed for malloc_stats */
#include <errno.h>

// #include <shlib-compat.h>

/* For uintptr_t.  */
#include <stdint.h>

/* For va_arg, va_start, va_end.  */
#include <stdarg.h>

/* For MIN, MAX, powerof2.  */
#include <sys/param.h>

/* For ALIGN_UP et. al.  */
// #include <libc-internal.h>

#include "types.h"


/*
  Debugging:

  Because freed chunks may be overwritten with bookkeeping fields, this
  malloc will often die when freed memory is overwritten by user
  programs.  This can be very effective (albeit in an annoying way)
  in helping track down dangling pointers.

  If you compile with -DMALLOC_DEBUG, a number of assertion checks are
  enabled that will catch more memory errors. You probably won't be
  able to make much sense of the actual assertion errors, but they
  should help you locate incorrectly overwritten memory.  The checking
  is fairly extensive, and will slow down execution
  noticeably. Calling malloc_stats or mallinfo with MALLOC_DEBUG set
  will attempt to check every non-mmapped allocated and free chunk in
  the course of computing the summmaries. (By nature, mmapped regions
  cannot be checked very much automatically.)

  Setting MALLOC_DEBUG may also be helpful if you are trying to modify
  this code. The assertions in the check routines spell out in more
  detail the assumptions and invariants underlying the algorithms.

  Setting MALLOC_DEBUG does NOT provide an automated mechanism for
  checking that all accesses to malloced memory stay within their
  bounds. However, there are several add-ons and adaptations of this
  or other mallocs available that do this.
*/

#ifndef MALLOC_DEBUG
#define MALLOC_DEBUG 0
#endif

#ifdef NDEBUG
# define assert(expr) ((void) 0)
#else
# define assert(expr) \
  ((expr)								      \
   ? ((void) 0)								      \
   : __malloc_assert (#expr, __FILE__, __LINE__, __func__))

extern const char *__progname;

static void
__malloc_assert (const char *assertion, const char *file, unsigned int line,
		 const char *function)
{
  (void) __fxprintf (NULL, "%s%s%s:%u: %s%sAssertion `%s' failed.\n",
		     __progname, __progname[0] ? ": " : "",
		     file, line,
		     function ? function : "", function ? ": " : "",
		     assertion);
  fflush (stderr);
  abort ();
}
#endif

/*
  REALLOC_ZERO_BYTES_FREES should be set if a call to
  realloc with zero bytes should be the same as a call to free.
  This is required by the C standard. Otherwise, since this malloc
  returns a unique pointer for malloc(0), so does realloc(p, 0).
*/

#ifndef REALLOC_ZERO_BYTES_FREES
#define REALLOC_ZERO_BYTES_FREES 1
#endif

/*
  TRIM_FASTBINS controls whether free() of a very small chunk can
  immediately lead to trimming. Setting to true (1) can reduce memory
  footprint, but will almost always slow down programs that use a lot
  of small chunks.

  Define this only if you are willing to give up some speed to more
  aggressively reduce system-level memory footprint when releasing
  memory in programs that use many small chunks.  You can get
  essentially the same effect by setting MXFAST to 0, but this can
  lead to even greater slowdowns in programs using many small chunks.
  TRIM_FASTBINS is an in-between compile-time option, that disables
  only those chunks bordering topmost memory from being placed in
  fastbins.
*/

#ifndef TRIM_FASTBINS
#define TRIM_FASTBINS  0
#endif



#include <string.h>

/*
  MORECORE-related declarations. By default, rely on sbrk
*/


/*
  MORECORE is the name of the routine to call to obtain more memory
  from the system.  See below for general guidance on writing
  alternative MORECORE functions, as well as a version for WIN32 and a
  sample version for pre-OSX macos.
*/

#ifndef MORECORE
#define MORECORE sbrk
#endif

/*
  MORECORE_FAILURE is the value returned upon failure of MORECORE
  as well as mmap. Since it cannot be an otherwise valid memory address,
  and must reflect values of standard sys calls, you probably ought not
  try to redefine it.
*/

#ifndef MORECORE_FAILURE
#define MORECORE_FAILURE (-1)
#endif

/*
  If MORECORE_CONTIGUOUS is true, take advantage of fact that
  consecutive calls to MORECORE with positive arguments always return
  contiguous increasing addresses.  This is true of unix sbrk.  Even
  if not defined, when regions happen to be contiguous, malloc will
  permit allocations spanning regions obtained from different
  calls. But defining this when applicable enables some stronger
  consistency checks and space efficiencies.
*/

#ifndef MORECORE_CONTIGUOUS
#define MORECORE_CONTIGUOUS 1
#endif

/*
  Define MORECORE_CANNOT_TRIM if your version of MORECORE
  cannot release space back to the system when given negative
  arguments. This is generally necessary only if you are using
  a hand-crafted MORECORE function that cannot handle negative arguments.
*/

/* #define MORECORE_CANNOT_TRIM */

/*  MORECORE_CLEARS           (default 1)
     The degree to which the routine mapped to MORECORE zeroes out
     memory: never (0), only for newly allocated space (1) or always
     (2).  The distinction between (1) and (2) is necessary because on
     some systems, if the application first decrements and then
     increments the break value, the contents of the reallocated space
     are unspecified.
 */

#ifndef MORECORE_CLEARS
# define MORECORE_CLEARS 1
#endif


/*
   MMAP_AS_MORECORE_SIZE is the minimum mmap size argument to use if
   sbrk fails, and mmap is used as a backup.  The value must be a
   multiple of page size.  This backup strategy generally applies only
   when systems have "holes" in address space, so sbrk cannot perform
   contiguous expansion, but there is still space available on system.
   On systems for which this is known to be useful (i.e. most linux
   kernels), this occurs only when programs allocate huge amounts of
   memory.  Between this, and the fact that mmap regions tend to be
   limited, the size should be large, to avoid too many mmap calls and
   thus avoid running out of kernel resources.  */

#ifndef MMAP_AS_MORECORE_SIZE
#define MMAP_AS_MORECORE_SIZE (1024 * 1024)
#endif

/*
  Define HAVE_MREMAP to make realloc() use mremap() to re-allocate
  large blocks.
*/

#ifndef HAVE_MREMAP
#define HAVE_MREMAP 0
#endif


/*
  This version of malloc supports the standard SVID/XPG mallinfo
  routine that returns a struct containing usage properties and
  statistics. It should work on any SVID/XPG compliant system that has
  a /usr/include/malloc.h defining struct mallinfo. (If you'd like to
  install such a thing yourself, cut out the preliminary declarations
  as described above and below and save them in a malloc.h file. But
  there's no compelling reason to bother to do this.)

  The main declaration needed is the mallinfo struct that is returned
  (by-copy) by mallinfo().  The SVID/XPG malloinfo struct contains a
  bunch of fields that are not even meaningful in this version of
  malloc.  These fields are are instead filled by mallinfo() with
  other numbers that might be of interest.
*/


/* ---------- description of public routines ------------ */

/*
  free(void* p)
  Releases the chunk of memory pointed to by p, that had been previously
  allocated using malloc or a related routine such as realloc.
  It has no effect if p is null. It can have arbitrary (i.e., bad!)
  effects if p has already been freed.

  Unless disabled (using mallopt), freeing very large spaces will
  when possible, automatically trigger operations that give
  back unused memory to the system, thus reducing program footprint.
*/
void     __libc_free(void*);
libc_hidden_proto (__libc_free)

/*
  calloc(size_t n_elements, size_t element_size);
  Returns a pointer to n_elements * element_size bytes, with all locations
  set to zero.
*/
void*  __libc_calloc(size_t, size_t);

/*
  valloc(size_t n);
  Equivalent to memalign(pagesize, n), where pagesize is the page
  size of the system. If the pagesize is unknown, 4096 is used.
*/
void*  __libc_valloc(size_t);



/*
  mallopt(int parameter_number, int parameter_value)
  Sets tunable parameters The format is to provide a
  (parameter-number, parameter-value) pair.  mallopt then sets the
  corresponding parameter to the argument value if it can (i.e., so
  long as the value is meaningful), and returns 1 if successful else
  0.  SVID/XPG/ANSI defines four standard param numbers for mallopt,
  normally defined in malloc.h.  Only one of these (M_MXFAST) is used
  in this malloc. The others (M_NLBLKS, M_GRAIN, M_KEEP) don't apply,
  so setting them has no effect. But this malloc also supports four
  other options in mallopt. See below for details.  Briefly, supported
  parameters are as follows (listed defaults are for "typical"
  configurations).

  Symbol            param #   default    allowed param values
  M_MXFAST          1         64         0-80  (0 disables fastbins)
  M_TRIM_THRESHOLD -1         128*1024   any   (-1U disables trimming)
  M_TOP_PAD        -2         0          any
  M_MMAP_THRESHOLD -3         128*1024   any   (or 0 if no MMAP support)
  M_MMAP_MAX       -4         65536      any   (0 disables use of mmap)
*/
int      __libc_mallopt(int, int);
libc_hidden_proto (__libc_mallopt)


/*
  mallinfo()
  Returns (by copy) a struct containing various summary statistics:

  arena:     current total non-mmapped bytes allocated from system
  ordblks:   the number of free chunks
  smblks:    the number of fastbin blocks (i.e., small chunks that
	       have been freed but not use resused or consolidated)
  hblks:     current number of mmapped regions
  hblkhd:    total bytes held in mmapped regions
  usmblks:   the maximum total allocated space. This will be greater
		than current total if trimming has occurred.
  fsmblks:   total bytes held in fastbin blocks
  uordblks:  current total allocated space (normal or mmapped)
  fordblks:  total free space
  keepcost:  the maximum number of bytes that could ideally be released
	       back to system via malloc_trim. ("ideally" means that
	       it ignores page restrictions etc.)

  Because these fields are ints, but internal bookkeeping may
  be kept as longs, the reported values may wrap around zero and
  thus be inaccurate.
*/
struct mallinfo __libc_mallinfo(void);


/*
  pvalloc(size_t n);
  Equivalent to valloc(minimum-page-that-holds(n)), that is,
  round up n to nearest pagesize.
 */
void*  __libc_pvalloc(size_t);

/*
  malloc_trim(size_t pad);

  If possible, gives memory back to the system (via negative
  arguments to sbrk) if there is unused memory at the `high' end of
  the malloc pool. You can call this after freeing large blocks of
  memory to potentially reduce the system-level memory requirements
  of a program. However, it cannot guarantee to reduce memory. Under
  some allocation patterns, some large free blocks of memory will be
  locked between two used chunks, so they cannot be given back to
  the system.

  The `pad' argument to malloc_trim represents the amount of free
  trailing space to leave untrimmed. If this argument is zero,
  only the minimum amount of memory to maintain internal data
  structures will be left (one page or less). Non-zero arguments
  can be supplied to maintain enough trailing space to service
  future expected allocations without having to re-obtain memory
  from the system.

  Malloc_trim returns 1 if it actually released any memory, else 0.
  On systems that do not support "negative sbrks", it will always
  return 0.
*/
int      __malloc_trim(size_t);

/*
  malloc_usable_size(void* p);

  Returns the number of bytes you can actually use in
  an allocated chunk, which may be more than you requested (although
  often not) due to alignment and minimum size constraints.
  You can use this many bytes without worrying about
  overwriting other allocated objects. This is not a particularly great
  programming practice. malloc_usable_size can be more useful in
  debugging and assertions, for example:

  p = malloc(n);
  assert(malloc_usable_size(p) >= 256);

*/
size_t   __malloc_usable_size(void*);

/*
  malloc_stats();
  Prints on stderr the amount of space obtained from the system (both
  via sbrk and mmap), the maximum amount (which may be more than
  current if malloc_trim and/or munmap got called), and the current
  number of bytes allocated via malloc (or realloc, etc) but not yet
  freed. Note that this is the number of bytes allocated, not the
  number requested. It will be larger than the number requested
  because of alignment and bookkeeping overhead. Because it includes
  alignment wastage as being in use, this figure may be greater than
  zero even when no user-level chunks are allocated.

  The reported current and maximum system memory can be inaccurate if
  a program makes other calls to system memory allocation functions
  (normally sbrk) outside of malloc.

  malloc_stats prints only the most commonly interesting statistics.
  More information can be obtained by calling mallinfo.

*/
void     __malloc_stats(void);

/*
  malloc_get_state(void);

  Returns the state of all malloc variables in an opaque data
  structure.
*/
void*  __malloc_get_state(void);

/*
  malloc_set_state(void* state);

  Restore the state of all malloc variables from data obtained with
  malloc_get_state().
*/
int      __malloc_set_state(void*);

/*
  posix_memalign(void **memptr, size_t alignment, size_t size);

  POSIX wrapper like memalign(), checking for validity of size.
*/
int      __posix_memalign(void **, size_t, size_t);

/* mallopt tuning options */

/*
  M_MXFAST is the maximum request size used for "fastbins", special bins
  that hold returned chunks without consolidating their spaces. This
  enables future requests for chunks of the same size to be handled
  very quickly, but can increase fragmentation, and thus increase the
  overall memory footprint of a program.

  This malloc manages fastbins very conservatively yet still
  efficiently, so fragmentation is rarely a problem for values less
  than or equal to the default.  The maximum supported value of MXFAST
  is 80. You wouldn't want it any higher than this anyway.  Fastbins
  are designed especially for use with many small structs, objects or
  strings -- the default handles structs/objects/arrays with sizes up
  to 8 4byte fields, or small strings representing words, tokens,
  etc. Using fastbins for larger objects normally worsens
  fragmentation without improving speed.

  M_MXFAST is set in REQUEST size units. It is internally used in
  chunksize units, which adds padding and alignment.  You can reduce
  M_MXFAST to 0 to disable all use of fastbins.  This causes the malloc
  algorithm to be a closer approximation of fifo-best-fit in all cases,
  not just for larger requests, but will generally cause it to be
  slower.
*/


/* M_MXFAST is a standard SVID/XPG tuning option, usually listed in malloc.h */
#ifndef M_MXFAST
#define M_MXFAST            1
#endif

#ifndef DEFAULT_MXFAST
#define DEFAULT_MXFAST     (64 * SIZE_SZ / 4)
#endif




#include <malloc.h>

#ifndef RETURN_ADDRESS
#define RETURN_ADDRESS(X_) (NULL)
#endif

/* Internal routines.  */



/* ------------------ MMAP support ------------------  */


#include <fcntl.h>
#include <sys/mman.h>

#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
# define MAP_ANONYMOUS MAP_ANON
#endif

#ifndef MAP_NORESERVE
# define MAP_NORESERVE 0
#endif

#define MMAP(addr, size, prot, flags) \
 __mmap((addr), (size), (prot), (flags)|MAP_ANONYMOUS|MAP_PRIVATE, -1, 0)





/*
   -------------------- Internal data structures --------------------

   All internal state is held in an instance of malloc_state defined
   below. There are no other static variables, except in two optional
   cases:
 * If USE_MALLOC_LOCK is defined, the mALLOC_MUTEx declared above.
 * If mmap doesn't support MAP_ANONYMOUS, a dummy file descriptor
     for mmap.

   Beware of lots of tricks that minimize the total bookkeeping space
   requirements. The result is a little over 1K bytes (for 4byte
   pointers and size_t.)
 */



/* ARENA_CORRUPTION_BIT is set if a memory corruption was detected on the
   arena.  Such an arena is no longer used to allocate chunks.  Chunks
   allocated in that arena before detecting corruption are not freed.  */

#define ARENA_CORRUPTION_BIT (4U)

#define arena_is_corrupt(A)	(((A)->flags & ARENA_CORRUPTION_BIT))
#define set_arena_corrupt(A)	((A)->flags |= ARENA_CORRUPTION_BIT)

/*
   Set value of max_fast.
   Use impossibly small value if 0.
   Precondition: there are no existing fastbin chunks.
   Setting the value clears fastchunk bit but preserves noncontiguous bit.
 */

#define set_max_fast(s) \
  global_max_fast = (((s) == 0)						      \
                     ? SMALLBIN_WIDTH : ((s + SIZE_SZ) & ~MALLOC_ALIGN_MASK))
#define get_max_fast() global_max_fast


/*  Non public mallopt parameters.  */
#define M_ARENA_TEST -7
#define M_ARENA_MAX  -8


/* Maximum size of memory handled in fastbins.  */
static INTERNAL_SIZE_T global_max_fast;

/*
   Initialize a malloc_state struct.

   This is called only from within malloc_consolidate, which needs
   be called in the same contexts anyway.  It is never called directly
   outside of malloc_consolidate because some optimizing compilers try
   to inline it at all call points, which turns out not to be an
   optimization at all. (Inlining it in malloc_consolidate is fine though.)
 */

static void
malloc_init_state (mstate av)
{
  int i;
  mbinptr bin;

  /* Establish circular links for normal bins */
  for (i = 1; i < NBINS; ++i)
    {
      bin = bin_at (av, i);
      bin->fd = bin->bk = bin;
    }

#if MORECORE_CONTIGUOUS
  if (av != &main_arena)
#endif
  set_noncontiguous (av);
  if (av == &main_arena)
    set_max_fast (DEFAULT_MXFAST);
  av->flags |= FASTCHUNKS_BIT;

  av->top = initial_top (av);
}

/*
   Other internal utilities operating on mstates
 */

static void *sysmalloc (INTERNAL_SIZE_T, mstate);
static int      systrim (size_t, mstate);
static void     malloc_consolidate (mstate);


/* -------------- Early definitions for debugging hooks ---------------- */

/* Define and initialize the hook variables.  These weak definitions must
   appear before any use of the variables in a function (arena.c uses one).  */
#ifndef weak_variable
/* In GNU libc we want the hook variables to be weak definitions to
   avoid a problem with Emacs.  */
# define weak_variable weak_function
#endif

/* Forward declarations.  */
static void *malloc_hook_ini (size_t sz,
                              const void *caller) __THROW;
static void *realloc_hook_ini (void *ptr, size_t sz,
                               const void *caller) __THROW;
static void *memalign_hook_ini (size_t alignment, size_t sz,
                                const void *caller) __THROW;

void weak_variable (*__malloc_initialize_hook) (void) = NULL;
void weak_variable (*__free_hook) (void *__ptr,
                                   const void *) = NULL;
void *weak_variable (*__malloc_hook)
  (size_t __size, const void *) = malloc_hook_ini;
void *weak_variable (*__realloc_hook)
  (void *__ptr, size_t __size, const void *)
  = realloc_hook_ini;
void *weak_variable (*__memalign_hook)
  (size_t __alignment, size_t __size, const void *)
  = memalign_hook_ini;
void weak_variable (*__after_morecore_hook) (void) = NULL;


/* ------------------ Testing support ----------------------------------*/

static int perturb_byte;

static void
alloc_perturb (char *p, size_t n)
{
  if (__glibc_unlikely (perturb_byte))
    memset (p, perturb_byte ^ 0xff, n);
}

static void
free_perturb (char *p, size_t n)
{
  if (__glibc_unlikely (perturb_byte))
    memset (p, perturb_byte, n);
}



// #include <stap-probe.h>

/* ------------------- Support for multiple arenas -------------------- */
#include "arena.cpp"

/*
   Debugging support

   These routines make a number of assertions about the states
   of data structures that should be true at all times. If any
   are not true, it's very likely that a user program has somehow
   trashed memory. (It's also possible that there is a coding error
   in malloc. In which case, please report it!)
 */

#if !MALLOC_DEBUG

# define check_chunk(A, P)
# define check_free_chunk(A, P)
# define check_inuse_chunk(A, P)
# define check_remalloced_chunk(A, P, N)
# define check_malloced_chunk(A, P, N)
# define check_malloc_state(A)

#else

# define check_chunk(A, P)              do_check_chunk (A, P)
# define check_free_chunk(A, P)         do_check_free_chunk (A, P)
# define check_inuse_chunk(A, P)        do_check_inuse_chunk (A, P)
# define check_remalloced_chunk(A, P, N) do_check_remalloced_chunk (A, P, N)
# define check_malloced_chunk(A, P, N)   do_check_malloced_chunk (A, P, N)
# define check_malloc_state(A)         do_check_malloc_state (A)

/*
   Properties of all chunks
 */

static void
do_check_chunk (mstate av, mchunkptr p)
{
  unsigned long sz = chunksize (p);
  /* min and max possible addresses assuming contiguous allocation */
  char *max_address = (char *) (av->top) + chunksize (av->top);
  char *min_address = max_address - av->system_mem;

  if (!chunk_is_mmapped (p))
    {
      /* Has legal address ... */
      if (p != av->top)
        {
          if (contiguous (av))
            {
              assert (((char *) p) >= min_address);
              assert (((char *) p + sz) <= ((char *) (av->top)));
            }
        }
      else
        {
          /* top size is always at least MINSIZE */
          assert ((unsigned long) (sz) >= MINSIZE);
          /* top predecessor always marked inuse */
          assert (prev_inuse (p));
        }
    }
  else
    {
      /* address is outside main heap  */
      if (contiguous (av) && av->top != initial_top (av))
        {
          assert (((char *) p) < min_address || ((char *) p) >= max_address);
        }
      /* chunk is page-aligned */
      assert (((p->prev_size + sz) & (GLRO (dl_pagesize) - 1)) == 0);
      /* mem is aligned */
      assert (aligned_OK (chunk2mem (p)));
    }
}

/*
   Properties of free chunks
 */

static void
do_check_free_chunk (mstate av, mchunkptr p)
{
  INTERNAL_SIZE_T sz = p->size & ~(PREV_INUSE | NON_MAIN_ARENA);
  mchunkptr next = chunk_at_offset (p, sz);

  do_check_chunk (av, p);

  /* Chunk must claim to be free ... */
  assert (!inuse (p));
  assert (!chunk_is_mmapped (p));

  /* Unless a special marker, must have OK fields */
  if ((unsigned long) (sz) >= MINSIZE)
    {
      assert ((sz & MALLOC_ALIGN_MASK) == 0);
      assert (aligned_OK (chunk2mem (p)));
      /* ... matching footer field */
      assert (next->prev_size == sz);
      /* ... and is fully consolidated */
      assert (prev_inuse (p));
      assert (next == av->top || inuse (next));

      /* ... and has minimally sane links */
      assert (p->fd->bk == p);
      assert (p->bk->fd == p);
    }
  else /* markers are always of size SIZE_SZ */
    assert (sz == SIZE_SZ);
}

/*
   Properties of inuse chunks
 */

static void
do_check_inuse_chunk (mstate av, mchunkptr p)
{
  mchunkptr next;

  do_check_chunk (av, p);

  if (chunk_is_mmapped (p))
    return; /* mmapped chunks have no next/prev */

  /* Check whether it claims to be in use ... */
  assert (inuse (p));

  next = next_chunk (p);

  /* ... and is surrounded by OK chunks.
     Since more things can be checked with free chunks than inuse ones,
     if an inuse chunk borders them and debug is on, it's worth doing them.
   */
  if (!prev_inuse (p))
    {
      /* Note that we cannot even look at prev unless it is not inuse */
      mchunkptr prv = prev_chunk (p);
      assert (next_chunk (prv) == p);
      do_check_free_chunk (av, prv);
    }

  if (next == av->top)
    {
      assert (prev_inuse (next));
      assert (chunksize (next) >= MINSIZE);
    }
  else if (!inuse (next))
    do_check_free_chunk (av, next);
}

/*
   Properties of chunks recycled from fastbins
 */

static void
do_check_remalloced_chunk (mstate av, mchunkptr p, INTERNAL_SIZE_T s)
{
  INTERNAL_SIZE_T sz = p->size & ~(PREV_INUSE | NON_MAIN_ARENA);

  if (!chunk_is_mmapped (p))
    {
      assert (av == arena_for_chunk (p));
      if (chunk_non_main_arena (p))
        assert (av != &main_arena);
      else
        assert (av == &main_arena);
    }

  do_check_inuse_chunk (av, p);

  /* Legal size ... */
  assert ((sz & MALLOC_ALIGN_MASK) == 0);
  assert ((unsigned long) (sz) >= MINSIZE);
  /* ... and alignment */
  assert (aligned_OK (chunk2mem (p)));
  /* chunk is less than MINSIZE more than request */
  assert ((long) (sz) - (long) (s) >= 0);
  assert ((long) (sz) - (long) (s + MINSIZE) < 0);
}

/*
   Properties of nonrecycled chunks at the point they are malloced
 */

static void
do_check_malloced_chunk (mstate av, mchunkptr p, INTERNAL_SIZE_T s)
{
  /* same as recycled case ... */
  do_check_remalloced_chunk (av, p, s);

  /*
     ... plus,  must obey implementation invariant that prev_inuse is
     always true of any allocated chunk; i.e., that each allocated
     chunk borders either a previously allocated and still in-use
     chunk, or the base of its memory arena. This is ensured
     by making all allocations from the `lowest' part of any found
     chunk.  This does not necessarily hold however for chunks
     recycled via fastbins.
   */

  assert (prev_inuse (p));
}


/*
   Properties of malloc_state.

   This may be useful for debugging malloc, as well as detecting user
   programmer errors that somehow write into malloc_state.

   If you are extending or experimenting with this malloc, you can
   probably figure out how to hack this routine to print out or
   display chunk addresses, sizes, bins, and other instrumentation.
 */

static void
do_check_malloc_state (mstate av)
{
  int i;
  mchunkptr p;
  mchunkptr q;
  mbinptr b;
  unsigned int idx;
  INTERNAL_SIZE_T size;
  unsigned long total = 0;
  int max_fast_bin;

  /* internal size_t must be no wider than pointer type */
  assert (sizeof (INTERNAL_SIZE_T) <= sizeof (char *));

  /* alignment is a power of 2 */
  assert ((MALLOC_ALIGNMENT & (MALLOC_ALIGNMENT - 1)) == 0);

  /* cannot run remaining checks until fully initialized */
  if (av->top == 0 || av->top == initial_top (av))
    return;

  /* pagesize is a power of 2 */
  assert (powerof2(GLRO (dl_pagesize)));

  /* A contiguous main_arena is consistent with sbrk_base.  */
  if (av == &main_arena && contiguous (av))
    assert ((char *) mp_.sbrk_base + av->system_mem ==
            (char *) av->top + chunksize (av->top));

  /* properties of fastbins */

  /* max_fast is in allowed range */
  assert ((get_max_fast () & ~1) <= request2size (MAX_FAST_SIZE));

  max_fast_bin = fastbin_index (get_max_fast ());

  for (i = 0; i < NFASTBINS; ++i)
    {
      p = fastbin (av, i);

      /* The following test can only be performed for the main arena.
         While mallopt calls malloc_consolidate to get rid of all fast
         bins (especially those larger than the new maximum) this does
         only happen for the main arena.  Trying to do this for any
         other arena would mean those arenas have to be locked and
         malloc_consolidate be called for them.  This is excessive.  And
         even if this is acceptable to somebody it still cannot solve
         the problem completely since if the arena is locked a
         concurrent malloc call might create a new arena which then
         could use the newly invalid fast bins.  */

      /* all bins past max_fast are empty */
      if (av == &main_arena && i > max_fast_bin)
        assert (p == 0);

      while (p != 0)
        {
          /* each chunk claims to be inuse */
          do_check_inuse_chunk (av, p);
          total += chunksize (p);
          /* chunk belongs in this bin */
          assert (fastbin_index (chunksize (p)) == i);
          p = p->fd;
        }
    }

  if (total != 0)
    assert (have_fastchunks (av));
  else if (!have_fastchunks (av))
    assert (total == 0);

  /* check normal bins */
  for (i = 1; i < NBINS; ++i)
    {
      b = bin_at (av, i);

      /* binmap is accurate (except for bin 1 == unsorted_chunks) */
      if (i >= 2)
        {
          unsigned int binbit = get_binmap (av, i);
          int empty = last (b) == b;
          if (!binbit)
            assert (empty);
          else if (!empty)
            assert (binbit);
        }

      for (p = last (b); p != b; p = p->bk)
        {
          /* each chunk claims to be free */
          do_check_free_chunk (av, p);
          size = chunksize (p);
          total += size;
          if (i >= 2)
            {
              /* chunk belongs in bin */
              idx = bin_index (size);
              assert (idx == i);
              /* lists are sorted */
              assert (p->bk == b ||
                      (unsigned long) chunksize (p->bk) >= (unsigned long) chunksize (p));

              if (!in_smallbin_range (size))
                {
                  if (p->fd_nextsize != NULL)
                    {
                      if (p->fd_nextsize == p)
                        assert (p->bk_nextsize == p);
                      else
                        {
                          if (p->fd_nextsize == first (b))
                            assert (chunksize (p) < chunksize (p->fd_nextsize));
                          else
                            assert (chunksize (p) > chunksize (p->fd_nextsize));

                          if (p == first (b))
                            assert (chunksize (p) > chunksize (p->bk_nextsize));
                          else
                            assert (chunksize (p) < chunksize (p->bk_nextsize));
                        }
                    }
                  else
                    assert (p->bk_nextsize == NULL);
                }
            }
          else if (!in_smallbin_range (size))
            assert (p->fd_nextsize == NULL && p->bk_nextsize == NULL);
          /* chunk is followed by a legal chain of inuse chunks */
          for (q = next_chunk (p);
               (q != av->top && inuse (q) &&
                (unsigned long) (chunksize (q)) >= MINSIZE);
               q = next_chunk (q))
            do_check_inuse_chunk (av, q);
        }
    }

  /* top chunk is OK */
  check_chunk (av, av->top);
}
#endif


/* ----------------- Support for debugging hooks -------------------- */
#include "hooks.c"


/* ----------- Routines dealing with system allocation -------------- */

/*
   sysmalloc handles malloc cases requiring more memory from the system.
   On entry, it is assumed that av->top does not have enough
   space to service request for nb bytes, thus requiring that av->top
   be extended or replaced.
 */

static void *
sysmalloc (INTERNAL_SIZE_T nb, mstate av)
{
  mchunkptr old_top;              /* incoming value of av->top */
  INTERNAL_SIZE_T old_size;       /* its size */
  char *old_end;                  /* its end address */

  long size;                      /* arg to first MORECORE or mmap call */
  char *brk;                      /* return value from MORECORE */

  long correction;                /* arg to 2nd MORECORE call */
  char *snd_brk;                  /* 2nd return val */

  INTERNAL_SIZE_T front_misalign; /* unusable bytes at front of new space */
  INTERNAL_SIZE_T end_misalign;   /* partial page left at end of new space */
  char *aligned_brk;              /* aligned offset into brk */

  mchunkptr p;                    /* the allocated/returned chunk */
  mchunkptr remainder;            /* remainder from allocation */
  unsigned long remainder_size;   /* its size */


  size_t pagesize = GLRO (dl_pagesize);
  bool tried_mmap = false;


  /*
     If have mmap, and the request size meets the mmap threshold, and
     the system supports mmap, and there are few enough currently
     allocated mmapped regions, try to directly map this request
     rather than expanding top.
   */

  if (av == NULL
      || ((unsigned long) (nb) >= (unsigned long) (mp_.mmap_threshold)
	  && (mp_.n_mmaps < mp_.n_mmaps_max)))
    {
      char *mm;           /* return value from mmap call*/

    try_mmap:
      /*
         Round up size to nearest page.  For mmapped chunks, the overhead
         is one SIZE_SZ unit larger than for normal chunks, because there
         is no following chunk whose prev_size field could be used.

         See the front_misalign handling below, for glibc there is no
         need for further alignments unless we have have high alignment.
       */
      if (MALLOC_ALIGNMENT == 2 * SIZE_SZ)
        size = ALIGN_UP (nb + SIZE_SZ, pagesize);
      else
        size = ALIGN_UP (nb + SIZE_SZ + MALLOC_ALIGN_MASK, pagesize);
      tried_mmap = true;

      /* Don't try if size wraps around 0 */
      if ((unsigned long) (size) > (unsigned long) (nb))
        {
          mm = (char *) (MMAP (0, size, PROT_READ | PROT_WRITE, 0));

          if (mm != MAP_FAILED)
            {
              /*
                 The offset to the start of the mmapped region is stored
                 in the prev_size field of the chunk. This allows us to adjust
                 returned start address to meet alignment requirements here
                 and in memalign(), and still be able to compute proper
                 address argument for later munmap in free() and realloc().
               */

              if (MALLOC_ALIGNMENT == 2 * SIZE_SZ)
                {
                  /* For glibc, chunk2mem increases the address by 2*SIZE_SZ and
                     MALLOC_ALIGN_MASK is 2*SIZE_SZ-1.  Each mmap'ed area is page
                     aligned and therefore definitely MALLOC_ALIGN_MASK-aligned.  */
                  assert (((INTERNAL_SIZE_T) chunk2mem (mm) & MALLOC_ALIGN_MASK) == 0);
                  front_misalign = 0;
                }
              else
                front_misalign = (INTERNAL_SIZE_T) chunk2mem (mm) & MALLOC_ALIGN_MASK;
              if (front_misalign > 0)
                {
                  correction = MALLOC_ALIGNMENT - front_misalign;
                  p = (mchunkptr) (mm + correction);
                  p->prev_size = correction;
                  set_head (p, (size - correction) | IS_MMAPPED);
                }
              else
                {
                  p = (mchunkptr) mm;
                  set_head (p, size | IS_MMAPPED);
                }

              /* update statistics */

              int new = atomic_exchange_and_add (&mp_.n_mmaps, 1) + 1;
              atomic_max (&mp_.max_n_mmaps, new);

              unsigned long sum;
              sum = atomic_exchange_and_add (&mp_.mmapped_mem, size) + size;
              atomic_max (&mp_.max_mmapped_mem, sum);

              check_chunk (av, p);

              return chunk2mem (p);
            }
        }
    }

  /* There are no usable arenas and mmap also failed.  */
  if (av == NULL)
    return 0;

  /* Record incoming configuration of top */

  old_top = av->top;
  old_size = chunksize (old_top);
  old_end = (char *) (chunk_at_offset (old_top, old_size));

  brk = snd_brk = (char *) (MORECORE_FAILURE);

  /*
     If not the first time through, we require old_size to be
     at least MINSIZE and to have prev_inuse set.
   */

  assert ((old_top == initial_top (av) && old_size == 0) ||
          ((unsigned long) (old_size) >= MINSIZE &&
           prev_inuse (old_top) &&
           ((unsigned long) old_end & (pagesize - 1)) == 0));

  /* Precondition: not enough current space to satisfy nb request */
  assert ((unsigned long) (old_size) < (unsigned long) (nb + MINSIZE));


  if (av != &main_arena)
    {
      heap_info *old_heap, *heap;
      size_t old_heap_size;

      /* First try to extend the current heap. */
      old_heap = heap_for_ptr (old_top);
      old_heap_size = old_heap->size;
      if ((long) (MINSIZE + nb - old_size) > 0
          && grow_heap (old_heap, MINSIZE + nb - old_size) == 0)
        {
          av->system_mem += old_heap->size - old_heap_size;
          arena_mem += old_heap->size - old_heap_size;
          set_head (old_top, (((char *) old_heap + old_heap->size) - (char *) old_top)
                    | PREV_INUSE);
        }
      else if ((heap = new_heap (nb + (MINSIZE + sizeof (*heap)), mp_.top_pad)))
        {
          /* Use a newly allocated heap.  */
          heap->ar_ptr = av;
          heap->prev = old_heap;
          av->system_mem += heap->size;
          arena_mem += heap->size;
          /* Set up the new top.  */
          top (av) = chunk_at_offset (heap, sizeof (*heap));
          set_head (top (av), (heap->size - sizeof (*heap)) | PREV_INUSE);

          /* Setup fencepost and free the old top chunk with a multiple of
             MALLOC_ALIGNMENT in size. */
          /* The fencepost takes at least MINSIZE bytes, because it might
             become the top chunk again later.  Note that a footer is set
             up, too, although the chunk is marked in use. */
          old_size = (old_size - MINSIZE) & ~MALLOC_ALIGN_MASK;
          set_head (chunk_at_offset (old_top, old_size + 2 * SIZE_SZ), 0 | PREV_INUSE);
          if (old_size >= MINSIZE)
            {
              set_head (chunk_at_offset (old_top, old_size), (2 * SIZE_SZ) | PREV_INUSE);
              set_foot (chunk_at_offset (old_top, old_size), (2 * SIZE_SZ));
              set_head (old_top, old_size | PREV_INUSE | NON_MAIN_ARENA);
              _int_free (av, old_top, 1);
            }
          else
            {
              set_head (old_top, (old_size + 2 * SIZE_SZ) | PREV_INUSE);
              set_foot (old_top, (old_size + 2 * SIZE_SZ));
            }
        }
      else if (!tried_mmap)
        /* We can at least try to use to mmap memory.  */
        goto try_mmap;
    }
  else     /* av == main_arena */


    { /* Request enough space for nb + pad + overhead */
      size = nb + mp_.top_pad + MINSIZE;

      /*
         If contiguous, we can subtract out existing space that we hope to
         combine with new space. We add it back later only if
         we don't actually get contiguous space.
       */

      if (contiguous (av))
        size -= old_size;

      /*
         Round to a multiple of page size.
         If MORECORE is not contiguous, this ensures that we only call it
         with whole-page arguments.  And if MORECORE is contiguous and
         this is not first time through, this preserves page-alignment of
         previous calls. Otherwise, we correct to page-align below.
       */

      size = ALIGN_UP (size, pagesize);

      /*
         Don't try to call MORECORE if argument is so big as to appear
         negative. Note that since mmap takes size_t arg, it may succeed
         below even if we cannot call MORECORE.
       */

      if (size > 0)
        {
          brk = (char *) (MORECORE (size));
          LIBC_PROBE (memory_sbrk_more, 2, brk, size);
        }

      if (brk != (char *) (MORECORE_FAILURE))
        {
          /* Call the `morecore' hook if necessary.  */
          void (*hook) (void) = atomic_forced_read (__after_morecore_hook);
          if (__builtin_expect (hook != NULL, 0))
            (*hook)();
        }
      else
        {
          /*
             If have mmap, try using it as a backup when MORECORE fails or
             cannot be used. This is worth doing on systems that have "holes" in
             address space, so sbrk cannot extend to give contiguous space, but
             space is available elsewhere.  Note that we ignore mmap max count
             and threshold limits, since the space will not be used as a
             segregated mmap region.
           */

          /* Cannot merge with old top, so add its size back in */
          if (contiguous (av))
            size = ALIGN_UP (size + old_size, pagesize);

          /* If we are relying on mmap as backup, then use larger units */
          if ((unsigned long) (size) < (unsigned long) (MMAP_AS_MORECORE_SIZE))
            size = MMAP_AS_MORECORE_SIZE;

          /* Don't try if size wraps around 0 */
          if ((unsigned long) (size) > (unsigned long) (nb))
            {
              char *mbrk = (char *) (MMAP (0, size, PROT_READ | PROT_WRITE, 0));

              if (mbrk != MAP_FAILED)
                {
                  /* We do not need, and cannot use, another sbrk call to find end */
                  brk = mbrk;
                  snd_brk = brk + size;

                  /*
                     Record that we no longer have a contiguous sbrk region.
                     After the first time mmap is used as backup, we do not
                     ever rely on contiguous space since this could incorrectly
                     bridge regions.
                   */
                  set_noncontiguous (av);
                }
            }
        }

      if (brk != (char *) (MORECORE_FAILURE))
        {
          if (mp_.sbrk_base == 0)
            mp_.sbrk_base = brk;
          av->system_mem += size;

          /*
             If MORECORE extends previous space, we can likewise extend top size.
           */

          if (brk == old_end && snd_brk == (char *) (MORECORE_FAILURE))
            set_head (old_top, (size + old_size) | PREV_INUSE);

          else if (contiguous (av) && old_size && brk < old_end)
            {
              /* Oops!  Someone else killed our space..  Can't touch anything.  */
              malloc_printerr (3, "break adjusted to free malloc space", brk,
			       av);
            }

          /*
             Otherwise, make adjustments:

           * If the first time through or noncontiguous, we need to call sbrk
              just to find out where the end of memory lies.

           * We need to ensure that all returned chunks from malloc will meet
              MALLOC_ALIGNMENT

           * If there was an intervening foreign sbrk, we need to adjust sbrk
              request size to account for fact that we will not be able to
              combine new space with existing space in old_top.

           * Almost all systems internally allocate whole pages at a time, in
              which case we might as well use the whole last page of request.
              So we allocate enough more memory to hit a page boundary now,
              which in turn causes future contiguous calls to page-align.
           */

          else
            {
              front_misalign = 0;
              end_misalign = 0;
              correction = 0;
              aligned_brk = brk;

              /* handle contiguous cases */
              if (contiguous (av))
                {
                  /* Count foreign sbrk as system_mem.  */
                  if (old_size)
                    av->system_mem += brk - old_end;

                  /* Guarantee alignment of first new chunk made from this space */

                  front_misalign = (INTERNAL_SIZE_T) chunk2mem (brk) & MALLOC_ALIGN_MASK;
                  if (front_misalign > 0)
                    {
                      /*
                         Skip over some bytes to arrive at an aligned position.
                         We don't need to specially mark these wasted front bytes.
                         They will never be accessed anyway because
                         prev_inuse of av->top (and any chunk created from its start)
                         is always true after initialization.
                       */

                      correction = MALLOC_ALIGNMENT - front_misalign;
                      aligned_brk += correction;
                    }

                  /*
                     If this isn't adjacent to existing space, then we will not
                     be able to merge with old_top space, so must add to 2nd request.
                   */

                  correction += old_size;

                  /* Extend the end address to hit a page boundary */
                  end_misalign = (INTERNAL_SIZE_T) (brk + size + correction);
                  correction += (ALIGN_UP (end_misalign, pagesize)) - end_misalign;

                  assert (correction >= 0);
                  snd_brk = (char *) (MORECORE (correction));

                  /*
                     If can't allocate correction, try to at least find out current
                     brk.  It might be enough to proceed without failing.

                     Note that if second sbrk did NOT fail, we assume that space
                     is contiguous with first sbrk. This is a safe assumption unless
                     program is multithreaded but doesn't use locks and a foreign sbrk
                     occurred between our first and second calls.
                   */

                  if (snd_brk == (char *) (MORECORE_FAILURE))
                    {
                      correction = 0;
                      snd_brk = (char *) (MORECORE (0));
                    }
                  else
                    {
                      /* Call the `morecore' hook if necessary.  */
                      void (*hook) (void) = atomic_forced_read (__after_morecore_hook);
                      if (__builtin_expect (hook != NULL, 0))
                        (*hook)();
                    }
                }

              /* handle non-contiguous cases */
              else
                {
                  if (MALLOC_ALIGNMENT == 2 * SIZE_SZ)
                    /* MORECORE/mmap must correctly align */
                    assert (((unsigned long) chunk2mem (brk) & MALLOC_ALIGN_MASK) == 0);
                  else
                    {
                      front_misalign = (INTERNAL_SIZE_T) chunk2mem (brk) & MALLOC_ALIGN_MASK;
                      if (front_misalign > 0)
                        {
                          /*
                             Skip over some bytes to arrive at an aligned position.
                             We don't need to specially mark these wasted front bytes.
                             They will never be accessed anyway because
                             prev_inuse of av->top (and any chunk created from its start)
                             is always true after initialization.
                           */

                          aligned_brk += MALLOC_ALIGNMENT - front_misalign;
                        }
                    }

                  /* Find out current end of memory */
                  if (snd_brk == (char *) (MORECORE_FAILURE))
                    {
                      snd_brk = (char *) (MORECORE (0));
                    }
                }

              /* Adjust top based on results of second sbrk */
              if (snd_brk != (char *) (MORECORE_FAILURE))
                {
                  av->top = (mchunkptr) aligned_brk;
                  set_head (av->top, (snd_brk - aligned_brk + correction) | PREV_INUSE);
                  av->system_mem += correction;

                  /*
                     If not the first time through, we either have a
                     gap due to foreign sbrk or a non-contiguous region.  Insert a
                     double fencepost at old_top to prevent consolidation with space
                     we don't own. These fenceposts are artificial chunks that are
                     marked as inuse and are in any case too small to use.  We need
                     two to make sizes and alignments work out.
                   */

                  if (old_size != 0)
                    {
                      /*
                         Shrink old_top to insert fenceposts, keeping size a
                         multiple of MALLOC_ALIGNMENT. We know there is at least
                         enough space in old_top to do this.
                       */
                      old_size = (old_size - 4 * SIZE_SZ) & ~MALLOC_ALIGN_MASK;
                      set_head (old_top, old_size | PREV_INUSE);

                      /*
                         Note that the following assignments completely overwrite
                         old_top when old_size was previously MINSIZE.  This is
                         intentional. We need the fencepost, even if old_top otherwise gets
                         lost.
                       */
                      chunk_at_offset (old_top, old_size)->size =
                        (2 * SIZE_SZ) | PREV_INUSE;

                      chunk_at_offset (old_top, old_size + 2 * SIZE_SZ)->size =
                        (2 * SIZE_SZ) | PREV_INUSE;

                      /* If possible, release the rest. */
                      if (old_size >= MINSIZE)
                        {
                          _int_free (av, old_top, 1);
                        }
                    }
                }
            }
        }
    } /* if (av !=  &main_arena) */

  if ((unsigned long) av->system_mem > (unsigned long) (av->max_system_mem))
    av->max_system_mem = av->system_mem;
  check_malloc_state (av);

  /* finally, do the allocation */
  p = av->top;
  size = chunksize (p);

  /* check that one of the above allocation paths succeeded */
  if ((unsigned long) (size) >= (unsigned long) (nb + MINSIZE))
    {
      remainder_size = size - nb;
      remainder = chunk_at_offset (p, nb);
      av->top = remainder;
      set_head (p, nb | PREV_INUSE | (av != &main_arena ? NON_MAIN_ARENA : 0));
      set_head (remainder, remainder_size | PREV_INUSE);
      check_malloced_chunk (av, p, nb);
      return chunk2mem (p);
    }

  /* catch all failure paths */
  __set_errno (ENOMEM);
  return 0;
}


/*
   systrim is an inverse of sorts to sysmalloc.  It gives memory back
   to the system (via negative arguments to sbrk) if there is unused
   memory at the `high' end of the malloc pool. It is called
   automatically by free() when top space exceeds the trim
   threshold. It is also called by the public malloc_trim routine.  It
   returns 1 if it actually released any memory, else 0.
 */

static int
systrim (size_t pad, mstate av)
{
  long top_size;         /* Amount of top-most memory */
  long extra;            /* Amount to release */
  long released;         /* Amount actually released */
  char *current_brk;     /* address returned by pre-check sbrk call */
  char *new_brk;         /* address returned by post-check sbrk call */
  size_t pagesize;
  long top_area;

  pagesize = GLRO (dl_pagesize);
  top_size = chunksize (av->top);

  top_area = top_size - MINSIZE - 1;
  if (top_area <= pad)
    return 0;

  /* Release in pagesize units and round down to the nearest page.  */
  extra = ALIGN_DOWN(top_area - pad, pagesize);

  if (extra == 0)
    return 0;

  /*
     Only proceed if end of memory is where we last set it.
     This avoids problems if there were foreign sbrk calls.
   */
  current_brk = (char *) (MORECORE (0));
  if (current_brk == (char *) (av->top) + top_size)
    {
      /*
         Attempt to release memory. We ignore MORECORE return value,
         and instead call again to find out where new end of memory is.
         This avoids problems if first call releases less than we asked,
         of if failure somehow altered brk value. (We could still
         encounter problems if it altered brk in some very bad way,
         but the only thing we can do is adjust anyway, which will cause
         some downstream failure.)
       */

      MORECORE (-extra);
      /* Call the `morecore' hook if necessary.  */
      void (*hook) (void) = atomic_forced_read (__after_morecore_hook);
      if (__builtin_expect (hook != NULL, 0))
        (*hook)();
      new_brk = (char *) (MORECORE (0));

      LIBC_PROBE (memory_sbrk_less, 2, new_brk, extra);

      if (new_brk != (char *) MORECORE_FAILURE)
        {
          released = (long) (current_brk - new_brk);

          if (released != 0)
            {
              /* Success. Adjust top. */
              av->system_mem -= released;
              set_head (av->top, (top_size - released) | PREV_INUSE);
              check_malloc_state (av);
              return 1;
            }
        }
    }
  return 0;
}

#if HAVE_MREMAP

static mchunkptr
mremap_chunk (mchunkptr p, size_t new_size)
{
  size_t pagesize = GLRO (dl_pagesize);
  INTERNAL_SIZE_T offset = p->prev_size;
  INTERNAL_SIZE_T size = chunksize (p);
  char *cp;

  assert (chunk_is_mmapped (p));
  assert (((size + offset) & (GLRO (dl_pagesize) - 1)) == 0);

  /* Note the extra SIZE_SZ overhead as in mmap_chunk(). */
  new_size = ALIGN_UP (new_size + offset + SIZE_SZ, pagesize);

  /* No need to remap if the number of pages does not change.  */
  if (size + offset == new_size)
    return p;

  cp = (char *) __mremap ((char *) p - offset, size + offset, new_size,
                          MREMAP_MAYMOVE);

  if (cp == MAP_FAILED)
    return 0;

  p = (mchunkptr) (cp + offset);

  assert (aligned_OK (chunk2mem (p)));

  assert ((p->prev_size == offset));
  set_head (p, (new_size - offset) | IS_MMAPPED);

  INTERNAL_SIZE_T new;
  new = atomic_exchange_and_add (&mp_.mmapped_mem, new_size - size - offset)
        + new_size - size - offset;
  atomic_max (&mp_.max_mmapped_mem, new);
  return p;
}
#endif /* HAVE_MREMAP */

/*------------------------ Public wrappers. --------------------------------*/

void
__libc_free (void *mem)
{
  mstate ar_ptr;
  mchunkptr p;                          /* chunk corresponding to mem */

  void (*hook) (void *, const void *)
    = atomic_forced_read (__free_hook);
  if (__builtin_expect (hook != NULL, 0))
    {
      (*hook)(mem, RETURN_ADDRESS (0));
      return;
    }

  if (mem == 0)                              /* free(0) has no effect */
    return;

  p = mem2chunk (mem);

  if (chunk_is_mmapped (p))                       /* release mmapped memory. */
    {
      /* see if the dynamic brk/mmap threshold needs adjusting */
      if (!mp_.no_dyn_threshold
          && p->size > mp_.mmap_threshold
          && p->size <= DEFAULT_MMAP_THRESHOLD_MAX)
        {
          mp_.mmap_threshold = chunksize (p);
          mp_.trim_threshold = 2 * mp_.mmap_threshold;
          LIBC_PROBE (memory_mallopt_free_dyn_thresholds, 2,
                      mp_.mmap_threshold, mp_.trim_threshold);
        }
      munmap_chunk (p);
      return;
    }

  ar_ptr = arena_for_chunk (p);
  _int_free (ar_ptr, p, 0);
}
libc_hidden_def (__libc_free)



static void *
_mid_memalign (size_t alignment, size_t bytes, void *address)
{
  mstate ar_ptr;
  void *p;

  void *(*hook) (size_t, size_t, const void *) =
    atomic_forced_read (__memalign_hook);
  if (__builtin_expect (hook != NULL, 0))
    return (*hook)(alignment, bytes, address);

  /* If we need less alignment than we give anyway, just relay to malloc.  */
  if (alignment <= MALLOC_ALIGNMENT)
    return __libc_malloc (bytes);

  /* Otherwise, ensure that it is at least a minimum chunk size */
  if (alignment < MINSIZE)
    alignment = MINSIZE;

  /* If the alignment is greater than SIZE_MAX / 2 + 1 it cannot be a
     power of 2 and will cause overflow in the check below.  */
  if (alignment > SIZE_MAX / 2 + 1)
    {
      __set_errno (EINVAL);
      return 0;
    }

  /* Check for overflow.  */
  if (bytes > SIZE_MAX - alignment - MINSIZE)
    {
      __set_errno (ENOMEM);
      return 0;
    }


  /* Make sure alignment is power of 2.  */
  if (!powerof2 (alignment))
    {
      size_t a = MALLOC_ALIGNMENT * 2;
      while (a < alignment)
        a <<= 1;
      alignment = a;
    }

  arena_get (ar_ptr, bytes + alignment + MINSIZE);

  p = _int_memalign (ar_ptr, alignment, bytes);
  if (!p && ar_ptr != NULL)
    {
      LIBC_PROBE (memory_memalign_retry, 2, bytes, alignment);
      ar_ptr = arena_get_retry (ar_ptr, bytes);
      p = _int_memalign (ar_ptr, alignment, bytes);
    }

  if (ar_ptr != NULL)
    (void) mutex_unlock (&ar_ptr->mutex);

  assert (!p || chunk_is_mmapped (mem2chunk (p)) ||
          ar_ptr == arena_for_chunk (mem2chunk (p)));
  return p;
}
/* For ISO C11.  */
weak_alias (__libc_memalign, aligned_alloc)
libc_hidden_def (__libc_memalign)

void *
__libc_valloc (size_t bytes)
{
  if (__malloc_initialized() < 0)
    ptmalloc_init ();

  void *address = RETURN_ADDRESS (0);
  size_t pagesize = GLRO (dl_pagesize);
  return _mid_memalign (pagesize, bytes, address);
}

void *
__libc_pvalloc (size_t bytes)
{
  if (__malloc_initialized() < 0)
    ptmalloc_init ();

  void *address = RETURN_ADDRESS (0);
  size_t pagesize = GLRO (dl_pagesize);
  size_t rounded_bytes = ALIGN_UP (bytes, pagesize);

  /* Check for overflow.  */
  if (bytes > SIZE_MAX - 2 * pagesize - MINSIZE)
    {
      __set_errno (ENOMEM);
      return 0;
    }

  return _mid_memalign (pagesize, rounded_bytes, address);
}

void *
__libc_calloc (size_t n, size_t elem_size)
{
  mstate av;
  mchunkptr oldtop, p;
  INTERNAL_SIZE_T bytes, sz, csz, oldtopsize;
  void *mem;
  unsigned long clearsize;
  unsigned long nclears;
  INTERNAL_SIZE_T *d;

  /* size_t is unsigned so the behavior on overflow is defined.  */
  bytes = n * elem_size;
#define HALF_INTERNAL_SIZE_T \
  (((INTERNAL_SIZE_T) 1) << (8 * sizeof (INTERNAL_SIZE_T) / 2))
  if (__builtin_expect ((n | elem_size) >= HALF_INTERNAL_SIZE_T, 0))
    {
      if (elem_size != 0 && bytes / elem_size != n)
        {
          __set_errno (ENOMEM);
          return 0;
        }
    }

  void *(*hook) (size_t, const void *) =
    atomic_forced_read (__malloc_hook);
  if (__builtin_expect (hook != NULL, 0))
    {
      sz = bytes;
      mem = (*hook)(sz, RETURN_ADDRESS (0));
      if (mem == 0)
        return 0;

      return memset (mem, 0, sz);
    }

  sz = bytes;

  arena_get (av, sz);
  if (av)
    {
      /* Check if we hand out the top chunk, in which case there may be no
	 need to clear. */
#if MORECORE_CLEARS
      oldtop = top (av);
      oldtopsize = chunksize (top (av));
# if MORECORE_CLEARS < 2
      /* Only newly allocated memory is guaranteed to be cleared.  */
      if (av == &main_arena &&
	  oldtopsize < mp_.sbrk_base + av->max_system_mem - (char *) oldtop)
	oldtopsize = (mp_.sbrk_base + av->max_system_mem - (char *) oldtop);
# endif
      if (av != &main_arena)
	{
	  heap_info *heap = heap_for_ptr (oldtop);
	  if (oldtopsize < (char *) heap + heap->mprotect_size - (char *) oldtop)
	    oldtopsize = (char *) heap + heap->mprotect_size - (char *) oldtop;
	}
#endif
    }
  else
    {
      /* No usable arenas.  */
      oldtop = 0;
      oldtopsize = 0;
    }
  mem = _int_malloc (av, sz);


  assert (!mem || chunk_is_mmapped (mem2chunk (mem)) ||
          av == arena_for_chunk (mem2chunk (mem)));

  if (mem == 0 && av != NULL)
    {
      LIBC_PROBE (memory_calloc_retry, 1, sz);
      av = arena_get_retry (av, sz);
      mem = _int_malloc (av, sz);
    }

  if (av != NULL)
    (void) mutex_unlock (&av->mutex);

  /* Allocation failed even after a retry.  */
  if (mem == 0)
    return 0;

  p = mem2chunk (mem);

  /* Two optional cases in which clearing not necessary */
  if (chunk_is_mmapped (p))
    {
      if (__builtin_expect (perturb_byte, 0))
        return memset (mem, 0, sz);

      return mem;
    }

  csz = chunksize (p);

#if MORECORE_CLEARS
  if (perturb_byte == 0 && (p == oldtop && csz > oldtopsize))
    {
      /* clear only the bytes from non-freshly-sbrked memory */
      csz = oldtopsize;
    }
#endif

  /* Unroll clear of <= 36 bytes (72 if 8byte sizes).  We know that
     contents have an odd number of INTERNAL_SIZE_T-sized words;
     minimally 3.  */
  d = (INTERNAL_SIZE_T *) mem;
  clearsize = csz - SIZE_SZ;
  nclears = clearsize / sizeof (INTERNAL_SIZE_T);
  assert (nclears >= 3);

  if (nclears > 9)
    return memset (d, 0, clearsize);

  else
    {
      *(d + 0) = 0;
      *(d + 1) = 0;
      *(d + 2) = 0;
      if (nclears > 4)
        {
          *(d + 3) = 0;
          *(d + 4) = 0;
          if (nclears > 6)
            {
              *(d + 5) = 0;
              *(d + 6) = 0;
              if (nclears > 8)
                {
                  *(d + 7) = 0;
                  *(d + 8) = 0;
                }
            }
        }
    }

  return mem;
}

/*
  ------------------------- malloc_consolidate -------------------------

  malloc_consolidate is a specialized version of free() that tears
  down chunks held in fastbins.  Free itself cannot be used for this
  purpose since, among other things, it might place chunks back onto
  fastbins.  So, instead, we need to use a minor variant of the same
  code.

  Also, because this routine needs to be called the first time through
  malloc anyway, it turns out to be the perfect place to trigger
  initialization code.
*/

static void malloc_consolidate(mstate av)
{
  mfastbinptr*    fb;                 /* current fastbin being consolidated */
  mfastbinptr*    maxfb;              /* last fastbin (for loop control) */
  mchunkptr       p;                  /* current chunk being consolidated */
  mchunkptr       nextp;              /* next chunk to consolidate */
  mchunkptr       unsorted_bin;       /* bin header */
  mchunkptr       first_unsorted;     /* chunk to link to */

  /* These have same use as in free() */
  mchunkptr       nextchunk;
  INTERNAL_SIZE_T size;
  INTERNAL_SIZE_T nextsize;
  INTERNAL_SIZE_T prevsize;
  int             nextinuse;
  mchunkptr       bck;
  mchunkptr       fwd;

  /*
    If max_fast is 0, we know that av hasn't
    yet been initialized, in which case do so below
  */

  if (get_max_fast () != 0) {
    clear_fastchunks(av);

    unsorted_bin = unsorted_chunks(av);

    /*
      Remove each chunk from fast bin and consolidate it, placing it
      then in unsorted bin. Among other reasons for doing this,
      placing in unsorted bin avoids needing to calculate actual bins
      until malloc is sure that chunks aren't immediately going to be
      reused anyway.
    */

    maxfb = &fastbin (av, NFASTBINS - 1);
    fb = &fastbin (av, 0);
    do {
      p = atomic_exchange_acq (fb, 0);
      if (p != 0) {
	do {
	  check_inuse_chunk(av, p);
	  nextp = p->fd;

	  /* Slightly streamlined version of consolidation code in free() */
	  size = p->size & ~(PREV_INUSE|NON_MAIN_ARENA);
	  nextchunk = chunk_at_offset(p, size);
	  nextsize = chunksize(nextchunk);

	  if (!prev_inuse(p)) {
	    prevsize = p->prev_size;
	    size += prevsize;
	    p = chunk_at_offset(p, -((long) prevsize));
	    unlink(av, p, bck, fwd);
	  }

	  if (nextchunk != av->top) {
	    nextinuse = inuse_bit_at_offset(nextchunk, nextsize);

	    if (!nextinuse) {
	      size += nextsize;
	      unlink(av, nextchunk, bck, fwd);
	    } else
	      clear_inuse_bit_at_offset(nextchunk, 0);

	    first_unsorted = unsorted_bin->fd;
	    unsorted_bin->fd = p;
	    first_unsorted->bk = p;

	    if (!in_smallbin_range (size)) {
	      p->fd_nextsize = NULL;
	      p->bk_nextsize = NULL;
	    }

	    set_head(p, size | PREV_INUSE);
	    p->bk = unsorted_bin;
	    p->fd = first_unsorted;
	    set_foot(p, size);
	  }

	  else {
	    size += nextsize;
	    set_head(p, size | PREV_INUSE);
	    av->top = p;
	  }

	} while ( (p = nextp) != 0);

      }
    } while (fb++ != maxfb);
  }
  else {
    malloc_init_state(av);
    check_malloc_state(av);
  }
}

/*
  ------------------------------ realloc ------------------------------
*/

void*
_int_realloc(mstate av, mchunkptr oldp, INTERNAL_SIZE_T oldsize,
	     INTERNAL_SIZE_T nb)
{
  mchunkptr        newp;            /* chunk to return */
  INTERNAL_SIZE_T  newsize;         /* its size */
  void*          newmem;          /* corresponding user mem */

  mchunkptr        next;            /* next contiguous chunk after oldp */

  mchunkptr        remainder;       /* extra space at end of newp */
  unsigned long    remainder_size;  /* its size */

  mchunkptr        bck;             /* misc temp for linking */
  mchunkptr        fwd;             /* misc temp for linking */

  unsigned long    copysize;        /* bytes to copy */
  unsigned int     ncopies;         /* INTERNAL_SIZE_T words to copy */
  INTERNAL_SIZE_T* s;               /* copy source */
  INTERNAL_SIZE_T* d;               /* copy destination */

  const char *errstr = NULL;

  /* oldmem size */
  if (__builtin_expect (oldp->size <= 2 * SIZE_SZ, 0)
      || __builtin_expect (oldsize >= av->system_mem, 0))
    {
      errstr = "realloc(): invalid old size";
    errout:
      malloc_printerr (check_action(), errstr, chunk2mem (oldp), av);
      return NULL;
    }

  check_inuse_chunk (av, oldp);

  /* All callers already filter out mmap'ed chunks.  */
  assert (!chunk_is_mmapped (oldp));

  next = chunk_at_offset (oldp, oldsize);
  INTERNAL_SIZE_T nextsize = chunksize (next);
  if (__builtin_expect (next->size <= 2 * SIZE_SZ, 0)
      || __builtin_expect (nextsize >= av->system_mem, 0))
    {
      errstr = "realloc(): invalid next size";
      goto errout;
    }

  if ((unsigned long) (oldsize) >= (unsigned long) (nb))
    {
      /* already big enough; split below */
      newp = oldp;
      newsize = oldsize;
    }

  else
    {
      /* Try to expand forward into top */
      if (next == av->top &&
          (unsigned long) (newsize = oldsize + nextsize) >=
          (unsigned long) (nb + MINSIZE))
        {
          set_head_size (oldp, nb | (av != &main_arena ? NON_MAIN_ARENA : 0));
          av->top = chunk_at_offset (oldp, nb);
          set_head (av->top, (newsize - nb) | PREV_INUSE);
          check_inuse_chunk (av, oldp);
          return chunk2mem (oldp);
        }

      /* Try to expand forward into next chunk;  split off remainder below */
      else if (next != av->top &&
               !inuse (next) &&
               (unsigned long) (newsize = oldsize + nextsize) >=
               (unsigned long) (nb))
        {
          newp = oldp;
          unlink (av, next, bck, fwd);
        }

      /* allocate, copy, free */
      else
        {
          newmem = _int_malloc (av, nb - MALLOC_ALIGN_MASK);
          if (newmem == 0)
            return 0; /* propagate failure */

          newp = mem2chunk (newmem);
          newsize = chunksize (newp);

          /*
             Avoid copy if newp is next chunk after oldp.
           */
          if (newp == next)
            {
              newsize += oldsize;
              newp = oldp;
            }
          else
            {
              /*
                 Unroll copy of <= 36 bytes (72 if 8byte sizes)
                 We know that contents have an odd number of
                 INTERNAL_SIZE_T-sized words; minimally 3.
               */

              copysize = oldsize - SIZE_SZ;
              s = (INTERNAL_SIZE_T *) (chunk2mem (oldp));
              d = (INTERNAL_SIZE_T *) (newmem);
              ncopies = copysize / sizeof (INTERNAL_SIZE_T);
              assert (ncopies >= 3);

              if (ncopies > 9)
                memcpy (d, s, copysize);

              else
                {
                  *(d + 0) = *(s + 0);
                  *(d + 1) = *(s + 1);
                  *(d + 2) = *(s + 2);
                  if (ncopies > 4)
                    {
                      *(d + 3) = *(s + 3);
                      *(d + 4) = *(s + 4);
                      if (ncopies > 6)
                        {
                          *(d + 5) = *(s + 5);
                          *(d + 6) = *(s + 6);
                          if (ncopies > 8)
                            {
                              *(d + 7) = *(s + 7);
                              *(d + 8) = *(s + 8);
                            }
                        }
                    }
                }

              _int_free (av, oldp, 1);
              check_inuse_chunk (av, newp);
              return chunk2mem (newp);
            }
        }
    }

  /* If possible, free extra space in old or extended chunk */

  assert ((unsigned long) (newsize) >= (unsigned long) (nb));

  remainder_size = newsize - nb;

  if (remainder_size < MINSIZE)   /* not enough extra to split off */
    {
      set_head_size (newp, newsize | (av != &main_arena ? NON_MAIN_ARENA : 0));
      set_inuse_bit_at_offset (newp, newsize);
    }
  else   /* split remainder */
    {
      remainder = chunk_at_offset (newp, nb);
      set_head_size (newp, nb | (av != &main_arena ? NON_MAIN_ARENA : 0));
      set_head (remainder, remainder_size | PREV_INUSE |
                (av != &main_arena ? NON_MAIN_ARENA : 0));
      /* Mark remainder as inuse so free() won't complain */
      set_inuse_bit_at_offset (remainder, remainder_size);
      _int_free (av, remainder, 1);
    }

  check_inuse_chunk (av, newp);
  return chunk2mem (newp);
}

/*
   ------------------------------ memalign ------------------------------
 */

static void *
_int_memalign (mstate av, size_t alignment, size_t bytes)
{
  INTERNAL_SIZE_T nb;             /* padded  request size */
  char *m;                        /* memory returned by malloc call */
  mchunkptr p;                    /* corresponding chunk */
  char *brk;                      /* alignment point within p */
  mchunkptr newp;                 /* chunk to return */
  INTERNAL_SIZE_T newsize;        /* its size */
  INTERNAL_SIZE_T leadsize;       /* leading space before alignment point */
  mchunkptr remainder;            /* spare room at end to split off */
  unsigned long remainder_size;   /* its size */
  INTERNAL_SIZE_T size;



  checked_request2size (bytes, nb);

  /*
     Strategy: find a spot within that chunk that meets the alignment
     request, and then possibly free the leading and trailing space.
   */


  /* Call malloc with worst case padding to hit alignment. */

  m = (char *) (_int_malloc (av, nb + alignment + MINSIZE));

  if (m == 0)
    return 0;           /* propagate failure */

  p = mem2chunk (m);

  if ((((unsigned long) (m)) % alignment) != 0)   /* misaligned */

    { /*
                Find an aligned spot inside chunk.  Since we need to give back
                leading space in a chunk of at least MINSIZE, if the first
                calculation places us at a spot with less than MINSIZE leader,
                we can move to the next aligned spot -- we've allocated enough
                total room so that this is always possible.
                 */
      brk = (char *) mem2chunk (((unsigned long) (m + alignment - 1)) &
                                - ((signed long) alignment));
      if ((unsigned long) (brk - (char *) (p)) < MINSIZE)
        brk += alignment;

      newp = (mchunkptr) brk;
      leadsize = brk - (char *) (p);
      newsize = chunksize (p) - leadsize;

      /* For mmapped chunks, just adjust offset */
      if (chunk_is_mmapped (p))
        {
          newp->prev_size = p->prev_size + leadsize;
          set_head (newp, newsize | IS_MMAPPED);
          return chunk2mem (newp);
        }

      /* Otherwise, give back leader, use the rest */
      set_head (newp, newsize | PREV_INUSE |
                (av != &main_arena ? NON_MAIN_ARENA : 0));
      set_inuse_bit_at_offset (newp, newsize);
      set_head_size (p, leadsize | (av != &main_arena ? NON_MAIN_ARENA : 0));
      _int_free (av, p, 1);
      p = newp;

      assert (newsize >= nb &&
              (((unsigned long) (chunk2mem (p))) % alignment) == 0);
    }

  /* Also give back spare room at the end */
  if (!chunk_is_mmapped (p))
    {
      size = chunksize (p);
      if ((unsigned long) (size) > (unsigned long) (nb + MINSIZE))
        {
          remainder_size = size - nb;
          remainder = chunk_at_offset (p, nb);
          set_head (remainder, remainder_size | PREV_INUSE |
                    (av != &main_arena ? NON_MAIN_ARENA : 0));
          set_head_size (p, nb);
          _int_free (av, remainder, 1);
        }
    }

  check_inuse_chunk (av, p);
  return chunk2mem (p);
}


/*
   ------------------------------ malloc_trim ------------------------------
 */

static int
mtrim (mstate av, size_t pad)
{
  /* Don't touch corrupt arenas.  */
  if (arena_is_corrupt (av))
    return 0;

  /* Ensure initialization/consolidation */
  malloc_consolidate (av);

  const size_t ps = GLRO (dl_pagesize);
  int psindex = bin_index (ps);
  const size_t psm1 = ps - 1;

  int result = 0;
  for (int i = 1; i < NBINS; ++i)
    if (i == 1 || i >= psindex)
      {
        mbinptr bin = bin_at (av, i);

        for (mchunkptr p = last (bin); p != bin; p = p->bk)
          {
            INTERNAL_SIZE_T size = chunksize (p);

            if (size > psm1 + sizeof (struct malloc_chunk))
              {
                /* See whether the chunk contains at least one unused page.  */
                char *paligned_mem = (char *) (((uintptr_t) p
                                                + sizeof (struct malloc_chunk)
                                                + psm1) & ~psm1);

                assert ((char *) chunk2mem (p) + 4 * SIZE_SZ <= paligned_mem);
                assert ((char *) p + size > paligned_mem);

                /* This is the size we could potentially free.  */
                size -= paligned_mem - (char *) p;

                if (size > psm1)
                  {
#if MALLOC_DEBUG
                    /* When debugging we simulate destroying the memory
                       content.  */
                    memset (paligned_mem, 0x89, size & ~psm1);
#endif
                    __madvise (paligned_mem, size & ~psm1, MADV_DONTNEED);

                    result = 1;
                  }
              }
          }
      }

#ifndef MORECORE_CANNOT_TRIM
  return result | (av == &main_arena ? systrim (pad, av) : 0);

#else
  return result;
#endif
}


int
__malloc_trim (size_t s)
{
  int result = 0;

  if (__malloc_initialized() < 0)
    ptmalloc_init ();

  mstate ar_ptr = &main_arena;
  do
    {
      (void) mutex_lock (&ar_ptr->mutex);
      result |= mtrim (ar_ptr, s);
      (void) mutex_unlock (&ar_ptr->mutex);

      ar_ptr = ar_ptr->next;
    }
  while (ar_ptr != &main_arena);

  return result;
}


/*
   ------------------------- malloc_usable_size -------------------------
 */

static size_t
musable (void *mem)
{
  mchunkptr p;
  if (mem != 0)
    {
      p = mem2chunk (mem);

      if (__builtin_expect (using_malloc_checking == 1, 0))
        return malloc_check_get_size (p);

      if (chunk_is_mmapped (p))
        return chunksize (p) - 2 * SIZE_SZ;
      else if (inuse (p))
        return chunksize (p) - SIZE_SZ;
    }
  return 0;
}


size_t
__malloc_usable_size (void *m)
{
  size_t result;

  result = musable (m);
  return result;
}

/*
   ------------------------------ mallinfo ------------------------------
   Accumulate malloc statistics for arena AV into M.
 */

static void
int_mallinfo (mstate av, struct mallinfo *m)
{
  size_t i;
  mbinptr b;
  mchunkptr p;
  INTERNAL_SIZE_T avail;
  INTERNAL_SIZE_T fastavail;
  int nblocks;
  int nfastblocks;

  /* Ensure initialization */
  if (av->top == 0)
    malloc_consolidate (av);

  check_malloc_state (av);

  /* Account for top */
  avail = chunksize (av->top);
  nblocks = 1;  /* top always exists */

  /* traverse fastbins */
  nfastblocks = 0;
  fastavail = 0;

  for (i = 0; i < NFASTBINS; ++i)
    {
      for (p = fastbin (av, i); p != 0; p = p->fd)
        {
          ++nfastblocks;
          fastavail += chunksize (p);
        }
    }

  avail += fastavail;

  /* traverse regular bins */
  for (i = 1; i < NBINS; ++i)
    {
      b = bin_at (av, i);
      for (p = last (b); p != b; p = p->bk)
        {
          ++nblocks;
          avail += chunksize (p);
        }
    }

  m->smblks += nfastblocks;
  m->ordblks += nblocks;
  m->fordblks += avail;
  m->uordblks += av->system_mem - avail;
  m->arena += av->system_mem;
  m->fsmblks += fastavail;
  if (av == &main_arena)
    {
      m->hblks = mp_.n_mmaps;
      m->hblkhd = mp_.mmapped_mem;
      m->usmblks = mp_.max_total_mem;
      m->keepcost = chunksize (av->top);
    }
}


struct mallinfo
__libc_mallinfo (void)
{
  struct mallinfo m;
  mstate ar_ptr;

  if (__malloc_initialized() < 0)
    ptmalloc_init ();

  memset (&m, 0, sizeof (m));
  ar_ptr = &main_arena;
  do
    {
      (void) mutex_lock (&ar_ptr->mutex);
      int_mallinfo (ar_ptr, &m);
      (void) mutex_unlock (&ar_ptr->mutex);

      ar_ptr = ar_ptr->next;
    }
  while (ar_ptr != &main_arena);

  return m;
}

/*
   ------------------------------ malloc_stats ------------------------------
 */

void
__malloc_stats (void)
{
  int i;
  mstate ar_ptr;
  unsigned int in_use_b = mp_.mmapped_mem, system_b = in_use_b;

  if (__malloc_initialized() < 0)
    ptmalloc_init ();
  _IO_flockfile (stderr);
  int old_flags2 = ((_IO_FILE *) stderr)->_flags2;
  ((_IO_FILE *) stderr)->_flags2 |= _IO_FLAGS2_NOTCANCEL;
  for (i = 0, ar_ptr = &main_arena;; i++)
    {
      struct mallinfo mi;

      memset (&mi, 0, sizeof (mi));
      (void) mutex_lock (&ar_ptr->mutex);
      int_mallinfo (ar_ptr, &mi);
      fprintf (stderr, "Arena %d:\n", i);
      fprintf (stderr, "system bytes     = %10u\n", (unsigned int) mi.arena);
      fprintf (stderr, "in use bytes     = %10u\n", (unsigned int) mi.uordblks);
#if MALLOC_DEBUG > 1
      if (i > 0)
        dump_heap (heap_for_ptr (top (ar_ptr)));
#endif
      system_b += mi.arena;
      in_use_b += mi.uordblks;
      (void) mutex_unlock (&ar_ptr->mutex);
      ar_ptr = ar_ptr->next;
      if (ar_ptr == &main_arena)
        break;
    }
  fprintf (stderr, "Total (incl. mmap):\n");
  fprintf (stderr, "system bytes     = %10u\n", system_b);
  fprintf (stderr, "in use bytes     = %10u\n", in_use_b);
  fprintf (stderr, "max mmap regions = %10u\n", (unsigned int) mp_.max_n_mmaps);
  fprintf (stderr, "max mmap bytes   = %10lu\n",
           (unsigned long) mp_.max_mmapped_mem);
  ((_IO_FILE *) stderr)->_flags2 |= old_flags2;
  _IO_funlockfile (stderr);
}


/*
   ------------------------------ mallopt ------------------------------
 */

int
__libc_mallopt (int param_number, int value)
{
  mstate av = &main_arena;
  int res = 1;

  if (__malloc_initialized() < 0)
    ptmalloc_init ();
  (void) mutex_lock (&av->mutex);
  /* Ensure initialization/consolidation */
  malloc_consolidate (av);

  LIBC_PROBE (memory_mallopt, 2, param_number, value);

  switch (param_number)
    {
    case M_MXFAST:
      if (value >= 0 && value <= MAX_FAST_SIZE)
        {
          LIBC_PROBE (memory_mallopt_mxfast, 2, value, get_max_fast ());
          set_max_fast (value);
        }
      else
        res = 0;
      break;

    case M_TRIM_THRESHOLD:
      LIBC_PROBE (memory_mallopt_trim_threshold, 3, value,
                  mp_.trim_threshold, mp_.no_dyn_threshold);
      mp_.trim_threshold = value;
      mp_.no_dyn_threshold = 1;
      break;

    case M_TOP_PAD:
      LIBC_PROBE (memory_mallopt_top_pad, 3, value,
                  mp_.top_pad, mp_.no_dyn_threshold);
      mp_.top_pad = value;
      mp_.no_dyn_threshold = 1;
      break;

    case M_MMAP_THRESHOLD:
      /* Forbid setting the threshold too high. */
      if ((unsigned long) value > HEAP_MAX_SIZE / 2)
        res = 0;
      else
        {
          LIBC_PROBE (memory_mallopt_mmap_threshold, 3, value,
                      mp_.mmap_threshold, mp_.no_dyn_threshold);
          mp_.mmap_threshold = value;
          mp_.no_dyn_threshold = 1;
        }
      break;

    case M_MMAP_MAX:
      LIBC_PROBE (memory_mallopt_mmap_max, 3, value,
                  mp_.n_mmaps_max, mp_.no_dyn_threshold);
      mp_.n_mmaps_max = value;
      mp_.no_dyn_threshold = 1;
      break;

    case M_CHECK_ACTION:
      LIBC_PROBE (memory_mallopt_check_action, 2, value, check_action());
      check_action() = value;
      break;

    case M_PERTURB:
      LIBC_PROBE (memory_mallopt_perturb, 2, value, perturb_byte);
      perturb_byte = value;
      break;

    case M_ARENA_TEST:
      if (value > 0)
        {
          LIBC_PROBE (memory_mallopt_arena_test, 2, value, mp_.arena_test);
          mp_.arena_test = value;
        }
      break;

    case M_ARENA_MAX:
      if (value > 0)
        {
          LIBC_PROBE (memory_mallopt_arena_max, 2, value, mp_.arena_max);
          mp_.arena_max = value;
        }
      break;
    }
  (void) mutex_unlock (&av->mutex);
  return res;
}
libc_hidden_def (__libc_mallopt)


/*
   -------------------- Alternative MORECORE functions --------------------
 */


/*
   General Requirements for MORECORE.

   The MORECORE function must have the following properties:

   If MORECORE_CONTIGUOUS is false:

 * MORECORE must allocate in multiples of pagesize. It will
      only be called with arguments that are multiples of pagesize.

 * MORECORE(0) must return an address that is at least
      MALLOC_ALIGNMENT aligned. (Page-aligning always suffices.)

   else (i.e. If MORECORE_CONTIGUOUS is true):

 * Consecutive calls to MORECORE with positive arguments
      return increasing addresses, indicating that space has been
      contiguously extended.

 * MORECORE need not allocate in multiples of pagesize.
      Calls to MORECORE need not have args of multiples of pagesize.

 * MORECORE need not page-align.

   In either case:

 * MORECORE may allocate more memory than requested. (Or even less,
      but this will generally result in a malloc failure.)

 * MORECORE must not allocate memory when given argument zero, but
      instead return one past the end address of memory from previous
      nonzero call. This malloc does NOT call MORECORE(0)
      until at least one call with positive arguments is made, so
      the initial value returned is not important.

 * Even though consecutive calls to MORECORE need not return contiguous
      addresses, it must be OK for malloc'ed chunks to span multiple
      regions in those cases where they do happen to be contiguous.

 * MORECORE need not handle negative arguments -- it may instead
      just return MORECORE_FAILURE when given negative arguments.
      Negative arguments are always multiples of pagesize. MORECORE
      must not misinterpret negative args as large positive unsigned
      args. You can suppress all such calls from even occurring by defining
      MORECORE_CANNOT_TRIM,

   There is some variation across systems about the type of the
   argument to sbrk/MORECORE. If size_t is unsigned, then it cannot
   actually be size_t, because sbrk supports negative args, so it is
   normally the signed type of the same width as size_t (sometimes
   declared as "intptr_t", and sometimes "ptrdiff_t").  It doesn't much
   matter though. Internally, we use "long" as arguments, which should
   work across all reasonable possibilities.

   Additionally, if MORECORE ever returns failure for a positive
   request, then mmap is used as a noncontiguous system allocator. This
   is a useful backup strategy for systems with holes in address spaces
   -- in this case sbrk cannot contiguously expand the heap, but mmap
   may be able to map noncontiguous space.

   If you'd like mmap to ALWAYS be used, you can define MORECORE to be
   a function that always returns MORECORE_FAILURE.

   If you are using this malloc with something other than sbrk (or its
   emulation) to supply memory regions, you probably want to set
   MORECORE_CONTIGUOUS as false.  As an example, here is a custom
   allocator kindly contributed for pre-OSX macOS.  It uses virtually
   but not necessarily physically contiguous non-paged memory (locked
   in, present and won't get swapped out).  You can use it by
   uncommenting this section, adding some #includes, and setting up the
   appropriate defines above:

 *#define MORECORE osMoreCore
 *#define MORECORE_CONTIGUOUS 0

   There is also a shutdown routine that should somehow be called for
   cleanup upon program exit.

 *#define MAX_POOL_ENTRIES 100
 *#define MINIMUM_MORECORE_SIZE  (64 * 1024)
   static int next_os_pool;
   void *our_os_pools[MAX_POOL_ENTRIES];

   void *osMoreCore(int size)
   {
    void *ptr = 0;
    static void *sbrk_top = 0;

    if (size > 0)
    {
      if (size < MINIMUM_MORECORE_SIZE)
         size = MINIMUM_MORECORE_SIZE;
      if (CurrentExecutionLevel() == kTaskLevel)
         ptr = PoolAllocateResident(size + RM_PAGE_SIZE, 0);
      if (ptr == 0)
      {
        return (void *) MORECORE_FAILURE;
      }
      // save ptrs so they can be freed during cleanup
      our_os_pools[next_os_pool] = ptr;
      next_os_pool++;
      ptr = (void *) ((((unsigned long) ptr) + RM_PAGE_MASK) & ~RM_PAGE_MASK);
      sbrk_top = (char *) ptr + size;
      return ptr;
    }
    else if (size < 0)
    {
      // we don't currently support shrink behavior
      return (void *) MORECORE_FAILURE;
    }
    else
    {
      return sbrk_top;
    }
   }

   // cleanup any allocated memory pools
   // called as last thing before shutting down driver

   void osCleanupMem(void)
   {
    void **ptr;

    for (ptr = our_os_pools; ptr < &our_os_pools[MAX_POOL_ENTRIES]; ptr++)
      if (*ptr)
      {
         PoolDeallocate(*ptr);
 * ptr = 0;
      }
   }

 */


/* Helper code.  */

extern char **__libc_argv attribute_hidden;

/* We need a wrapper function for one of the additions of POSIX.  */
int
__posix_memalign (void **memptr, size_t alignment, size_t size)
{
  void *mem;

  /* Test whether the SIZE argument is valid.  It must be a power of
     two multiple of sizeof (void *).  */
  if (alignment % sizeof (void *) != 0
      || !powerof2 (alignment / sizeof (void *))
      || alignment == 0)
    return EINVAL;


  void *address = RETURN_ADDRESS (0);
  mem = _mid_memalign (alignment, size, address);

  if (mem != NULL)
    {
      *memptr = mem;
      return 0;
    }

  return ENOMEM;
}
weak_alias (__posix_memalign, posix_memalign)


int
__malloc_info (int options, FILE *fp)
{
  /* For now, at least.  */
  if (options != 0)
    return EINVAL;

  int n = 0;
  size_t total_nblocks = 0;
  size_t total_nfastblocks = 0;
  size_t total_avail = 0;
  size_t total_fastavail = 0;
  size_t total_system = 0;
  size_t total_max_system = 0;
  size_t total_aspace = 0;
  size_t total_aspace_mprotect = 0;



  if (__malloc_initialized() < 0)
    ptmalloc_init ();

  fputs ("<malloc version=\"1\">\n", fp);

  /* Iterate over all arenas currently in use.  */
  mstate ar_ptr = &main_arena;
  do
    {
      fprintf (fp, "<heap nr=\"%d\">\n<sizes>\n", n++);

      size_t nblocks = 0;
      size_t nfastblocks = 0;
      size_t avail = 0;
      size_t fastavail = 0;
      struct
      {
	size_t from;
	size_t to;
	size_t total;
	size_t count;
      } sizes[NFASTBINS + NBINS - 1];
#define nsizes (sizeof (sizes) / sizeof (sizes[0]))

      mutex_lock (&ar_ptr->mutex);

      for (size_t i = 0; i < NFASTBINS; ++i)
	{
	  mchunkptr p = fastbin (ar_ptr, i);
	  if (p != NULL)
	    {
	      size_t nthissize = 0;
	      size_t thissize = chunksize (p);

	      while (p != NULL)
		{
		  ++nthissize;
		  p = p->fd;
		}

	      fastavail += nthissize * thissize;
	      nfastblocks += nthissize;
	      sizes[i].from = thissize - (MALLOC_ALIGNMENT - 1);
	      sizes[i].to = thissize;
	      sizes[i].count = nthissize;
	    }
	  else
	    sizes[i].from = sizes[i].to = sizes[i].count = 0;

	  sizes[i].total = sizes[i].count * sizes[i].to;
	}


      mbinptr bin;
      struct malloc_chunk *r;

      for (size_t i = 1; i < NBINS; ++i)
	{
	  bin = bin_at (ar_ptr, i);
	  r = bin->fd;
	  sizes[NFASTBINS - 1 + i].from = ~((size_t) 0);
	  sizes[NFASTBINS - 1 + i].to = sizes[NFASTBINS - 1 + i].total
					  = sizes[NFASTBINS - 1 + i].count = 0;

	  if (r != NULL)
	    while (r != bin)
	      {
		++sizes[NFASTBINS - 1 + i].count;
		sizes[NFASTBINS - 1 + i].total += r->size;
		sizes[NFASTBINS - 1 + i].from
		  = MIN (sizes[NFASTBINS - 1 + i].from, r->size);
		sizes[NFASTBINS - 1 + i].to = MAX (sizes[NFASTBINS - 1 + i].to,
						   r->size);

		r = r->fd;
	      }

	  if (sizes[NFASTBINS - 1 + i].count == 0)
	    sizes[NFASTBINS - 1 + i].from = 0;
	  nblocks += sizes[NFASTBINS - 1 + i].count;
	  avail += sizes[NFASTBINS - 1 + i].total;
	}

      mutex_unlock (&ar_ptr->mutex);

      total_nfastblocks += nfastblocks;
      total_fastavail += fastavail;

      total_nblocks += nblocks;
      total_avail += avail;

      for (size_t i = 0; i < nsizes; ++i)
	if (sizes[i].count != 0 && i != NFASTBINS)
	  fprintf (fp, "							      \
  <size from=\"%zu\" to=\"%zu\" total=\"%zu\" count=\"%zu\"/>\n",
		   sizes[i].from, sizes[i].to, sizes[i].total, sizes[i].count);

      if (sizes[NFASTBINS].count != 0)
	fprintf (fp, "\
  <unsorted from=\"%zu\" to=\"%zu\" total=\"%zu\" count=\"%zu\"/>\n",
		 sizes[NFASTBINS].from, sizes[NFASTBINS].to,
		 sizes[NFASTBINS].total, sizes[NFASTBINS].count);

      total_system += ar_ptr->system_mem;
      total_max_system += ar_ptr->max_system_mem;

      fprintf (fp,
	       "</sizes>\n<total type=\"fast\" count=\"%zu\" size=\"%zu\"/>\n"
	       "<total type=\"rest\" count=\"%zu\" size=\"%zu\"/>\n"
	       "<system type=\"current\" size=\"%zu\"/>\n"
	       "<system type=\"max\" size=\"%zu\"/>\n",
	       nfastblocks, fastavail, nblocks, avail,
	       ar_ptr->system_mem, ar_ptr->max_system_mem);

      if (ar_ptr != &main_arena)
	{
	  heap_info *heap = heap_for_ptr (top (ar_ptr));
	  fprintf (fp,
		   "<aspace type=\"total\" size=\"%zu\"/>\n"
		   "<aspace type=\"mprotect\" size=\"%zu\"/>\n",
		   heap->size, heap->mprotect_size);
	  total_aspace += heap->size;
	  total_aspace_mprotect += heap->mprotect_size;
	}
      else
	{
	  fprintf (fp,
		   "<aspace type=\"total\" size=\"%zu\"/>\n"
		   "<aspace type=\"mprotect\" size=\"%zu\"/>\n",
		   ar_ptr->system_mem, ar_ptr->system_mem);
	  total_aspace += ar_ptr->system_mem;
	  total_aspace_mprotect += ar_ptr->system_mem;
	}

      fputs ("</heap>\n", fp);
      ar_ptr = ar_ptr->next;
    }
  while (ar_ptr != &main_arena);

  fprintf (fp,
	   "<total type=\"fast\" count=\"%zu\" size=\"%zu\"/>\n"
	   "<total type=\"rest\" count=\"%zu\" size=\"%zu\"/>\n"
	   "<total type=\"mmap\" count=\"%d\" size=\"%zu\"/>\n"
	   "<system type=\"current\" size=\"%zu\"/>\n"
	   "<system type=\"max\" size=\"%zu\"/>\n"
	   "<aspace type=\"total\" size=\"%zu\"/>\n"
	   "<aspace type=\"mprotect\" size=\"%zu\"/>\n"
	   "</malloc>\n",
	   total_nfastblocks, total_fastavail, total_nblocks, total_avail,
	   mp_.n_mmaps, mp_.mmapped_mem,
	   total_system, total_max_system,
	   total_aspace, total_aspace_mprotect);

  return 0;
}
weak_alias (__malloc_info, malloc_info)


strong_alias (__libc_calloc, __calloc) weak_alias (__libc_calloc, calloc)
strong_alias (__libc_free, __cfree) weak_alias (__libc_free, cfree)
strong_alias (__libc_free, __free) strong_alias (__libc_free, free)
strong_alias (__libc_malloc, __malloc) strong_alias (__libc_malloc, malloc)
strong_alias (__libc_memalign, __memalign)
weak_alias (__libc_memalign, memalign)
strong_alias (__libc_realloc, __realloc) strong_alias (__libc_realloc, realloc)
strong_alias (__libc_valloc, __valloc) weak_alias (__libc_valloc, valloc)
strong_alias (__libc_pvalloc, __pvalloc) weak_alias (__libc_pvalloc, pvalloc)
strong_alias (__libc_mallinfo, __mallinfo)
weak_alias (__libc_mallinfo, mallinfo)
strong_alias (__libc_mallopt, __mallopt) weak_alias (__libc_mallopt, mallopt)

weak_alias (__malloc_stats, malloc_stats)
weak_alias (__malloc_usable_size, malloc_usable_size)
weak_alias (__malloc_trim, malloc_trim)
weak_alias (__malloc_get_state, malloc_get_state)
weak_alias (__malloc_set_state, malloc_set_state)


/* ------------------------------------------------------------
   History:

   [see ftp://g.oswego.edu/pub/misc/malloc.c for the history of dlmalloc]

 */
/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */

#pragma once

# define __MALLOC_DEPRECATED __attribute_deprecated__

/* ---------------- Error behavior ------------------------------------ */

#ifndef DEFAULT_CHECK_ACTION
# define DEFAULT_CHECK_ACTION 3
#endif

/*
   NONCONTIGUOUS_BIT indicates that MORECORE does not return contiguous
   regions.  Otherwise, contiguity is exploited in merging together,
   when possible, results from consecutive MORECORE calls.

   The initial value comes from MORECORE_CONTIGUOUS, but is
   changed dynamically if mmap is ever used as an sbrk substitute.
 */

#define NONCONTIGUOUS_BIT     (2U)

#define contiguous(M)          (((M)->flags & NONCONTIGUOUS_BIT) == 0)
#define noncontiguous(M)       (((M)->flags & NONCONTIGUOUS_BIT) != 0)
#define set_noncontiguous(M)   ((M)->flags |= NONCONTIGUOUS_BIT)
#define set_contiguous(M)      ((M)->flags &= ~NONCONTIGUOUS_BIT)

// ----------------------------------------------------------------------------

#define NARENAS_FROM_NCORES(n) ((n) * (sizeof (long) == 4 ? 2 : 8))

#define GLRO(x) _##x

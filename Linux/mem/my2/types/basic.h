#pragma once

#include <cstddef>

/*
  INTERNAL_SIZE_T is the word-size used for internal bookkeeping
  of chunk sizes.

  The default version is the same as size_t.

  While not strictly necessary, it is best to define this as an
  unsigned type, even if size_t is a signed type. This may avoid some
  artificial size limitations on some systems.

  On a 64-bit machine, you may be able to reduce malloc overhead by
  defining INTERNAL_SIZE_T to be a 32 bit `unsigned int' at the
  expense of not being able to handle more than 2^32 of malloced
  space. If this limitation is acceptable, you are encouraged to set
  this unless you are on a platform requiring 16byte alignments. In
  this case the alignment requirements turn out to negate any
  potential advantages of decreasing size_t word size.

  Implementors: Beware of the possible combinations of:
     - INTERNAL_SIZE_T might be signed or unsigned, might be 32 or 64 bits,
       and might be the same width as int or as long
     - size_t might have different width and signedness as INTERNAL_SIZE_T
     - int and long might be 32 or 64 bits, and might be the same width
  To deal with this, most comparisons and difference computations
  among INTERNAL_SIZE_Ts should cast them to unsigned long, being
  aware of the fact that casting an unsigned int to a wider long does
  not sign-extend. (This also makes checking for negative numbers
  awkward.) Some of these casts result in harmless compiler warnings
  on some systems.
*/
typedef size_t INTERNAL_SIZE_T;

/* The corresponding word size */
const size_t SIZE_SZ = sizeof(INTERNAL_SIZE_T);

/*
  MALLOC_ALIGNMENT is the minimum alignment for malloc'ed chunks.
  It must be a power of two at least 2 * SIZE_SZ, even on machines
  for which smaller alignments would suffice. It may be defined as
  larger than this though. Note however that code and data structures
  are optimized for the case of 8-byte alignment.
*/
const size_t MALLOC_ALIGNMENT = 2 *SIZE_SZ;

/* The corresponding bit mask value */
const size_t MALLOC_ALIGN_MASK = MALLOC_ALIGNMENT - 1;

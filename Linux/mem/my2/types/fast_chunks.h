#pragma once

/*
   Since the lowest 2 bits in max_fast don't matter in size comparisons,
   they are used as flags.
 */

/*
   FASTCHUNKS_BIT held in max_fast indicates that there are probably
   some fastbin chunks. It is set true on entering a chunk into any
   fastbin, and cleared only in malloc_consolidate.

   The truth value is inverted so that have_fastchunks will be true
   upon startup (since statics are zero-filled), simplifying
   initialization checks.
 */

#define FASTCHUNKS_BIT        (1U)

#define have_fastchunks(M)     (((M)->flags & FASTCHUNKS_BIT) == 0)
#define clear_fastchunks(M)    catomic_or (&(M)->flags, FASTCHUNKS_BIT)
#define set_fastchunks(M)      catomic_and (&(M)->flags, ~FASTCHUNKS_BIT)

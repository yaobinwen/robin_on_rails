#pragma once

#include "types/heap_info.h"

/* arena_get() acquires an arena and locks the corresponding mutex.
   First, try the one last locked successfully by this thread.  (This
   is the common case and handled with a macro for speed.)  Then, loop
   once over the circularly linked list of arenas.  If no arena is
   readily available, create a new one.  In this latter case, `size'
   is just a hint as to how much memory will be required immediately
   in the new arena. */

#define arena_get(ptr, size) do { \
      ptr = thread_arena; 		      \
      arena_lock (ptr, size);     \
  } while (0)

#define arena_lock(ptr, size) do { 	      \
      if (ptr && !arena_is_corrupt (ptr))  \
        (void) mutex_lock (&ptr->mutex); \
      else \
        ptr = arena_get2 ((size), NULL); \
  } while (0)

/* find the heap and corresponding arena for a given ptr */

#define heap_for_ptr(ptr) \
  ((heap_info *) ((unsigned long) (ptr) & ~(HEAP_MAX_SIZE - 1)))
#define arena_for_chunk(ptr) \
  (chunk_non_main_arena (ptr) ? heap_for_ptr (ptr)->ar_ptr : &main_arena())

#include <cstddef>

#include "fork.h"
#include "globals.h"
#include "hooks.h"
#include "malloc_int.h"
#include "types/malloc_chunk.h"
#include "arena.h"

// Just define it as NULL.
void *__dso_handle = nullptr;

void *(*save_malloc_hook)(size_t __size, const void *);
void (*save_free_hook) (void *__ptr, const void *);
void *save_arena;

void __linkin_atfork (struct fork_handler *newp) {
    // TODO: Implement me!
}

void * malloc_atfork (size_t sz, const void *caller)
{
  void *victim;

  if (thread_arena == ATFORK_ARENA_PTR)
    {
      /* We are the only thread that may allocate at all.  */
      if (save_malloc_hook != malloc_check)
        {
          return _int_malloc (&main_arena(), sz);
        }
      else
        {
          if (top_check () < 0)
            return 0;

          victim = _int_malloc (&main_arena(), sz + 1);
          return mem2mem_check (victim, sz);
        }
    }
  else
    {
      /* Suspend the thread until the `atfork' handlers have completed.
         By that time, the hooks will have been reset as well, so that
         mALLOc() can be used again. */
      (void) mutex_lock (&list_lock);
      (void) mutex_unlock (&list_lock);
      return __libc_malloc (sz);
    }
}

void free_atfork (void *mem, const void *caller)
{
  mstate ar_ptr;
  mchunkptr p;                          /* chunk corresponding to mem */

  if (mem == 0)                              /* free(0) has no effect */
    return;

  p = mem2chunk (mem);         /* do not bother to replicate free_check here */

  if (chunk_is_mmapped (p))                       /* release mmapped memory. */
    {
      munmap_chunk (p);
      return;
    }

  ar_ptr = arena_for_chunk (p);
  _int_free (ar_ptr, p, thread_arena == ATFORK_ARENA_PTR);
}

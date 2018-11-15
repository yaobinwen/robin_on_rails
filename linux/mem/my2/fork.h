#pragma once

#include <cstddef>

#ifndef NO_THREADS

/* This is defined by newer gcc version unique for each module.  */
extern void *__dso_handle __attribute__ ((__weak__));

/* Elements of the fork handler lists.  */
struct fork_handler
{
  struct fork_handler *next;
  void (*prepare_handler) (void);
  void (*parent_handler) (void);
  void (*child_handler) (void);
  void *dso_handle;
  unsigned int refcntr;
  int need_signal;
};

extern void *(*save_malloc_hook)(size_t __size, const void *);
extern void (*save_free_hook) (void *__ptr, const void *);
extern void *save_arena;

/* Magic value for the thread-specific arena pointer when
   malloc_atfork() is in use.  */

# define ATFORK_ARENA_PTR ((void *) -1)

#define ATFORK_MEM static struct fork_handler atfork_mem

void __linkin_atfork (struct fork_handler *newp);

#ifdef SHARED
# define thread_atfork(prepare, parent, child) \
  atfork_mem().prepare_handler = prepare;					      \
  atfork_mem().parent_handler = parent;					      \
  atfork_mem().child_handler = child;					      \
  atfork_mem().dso_handle = __dso_handle;					      \
  atfork_mem().refcntr = 1;						      \
  __linkin_atfork (&atfork_mem())
#else
# define thread_atfork(prepare, parent, child) \
  atfork_mem().prepare_handler = prepare;					      \
  atfork_mem().parent_handler = parent;					      \
  atfork_mem().child_handler = child;					      \
  atfork_mem().dso_handle = &__dso_handle == NULL ? NULL : __dso_handle;	      \
  atfork_mem().refcntr = 1;						      \
  __linkin_atfork (&atfork_mem())
#endif

/* The following hooks are used while the `atfork' handling mechanism
   is active. */
void * malloc_atfork (size_t sz, const void *caller);
void free_atfork (void *mem, const void *caller);

#endif  /* !NO_THREADS */

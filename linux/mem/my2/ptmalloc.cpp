#include <cstring>
#include <cstdlib>

#include "ptmalloc.h"
#include "malloc.h"
#include "globals.h"
#include "fork.h"
#include "env.h"
#include "types/top_pad.h"
#include "hooks.h"
#include "atomic.h"

void ptmalloc_init (void)
{
  if (__malloc_initialized() >= 0)
    return;

  __malloc_initialized() = 0;

#ifdef SHARED
  /* In case this libc copy is in a non-default namespace, never use brk.
     Likewise if dlopened from statically linked program.  */
  Dl_info di;
  struct link_map *l;

  if (_dl_open_hook != NULL
      || (_dl_addr (ptmalloc_init, &di, &l, NULL) != 0
          && l->l_ns != LM_ID_BASE))
    __morecore = __failing_morecore;
#endif

  thread_arena = &main_arena();
  thread_atfork (ptmalloc_lock_all, ptmalloc_unlock_all, ptmalloc_unlock_all2);
  const char *s = NULL;
  if (__glibc_likely (_environ != NULL))
    {
      char **runp = _environ;
      char *envline;

      while (__builtin_expect ((envline = next_env_entry (&runp)) != NULL,
                               0))
        {
          size_t len = strcspn (envline, "=");

          if (envline[len] != '=')
            // This is a "MALLOC_" variable at the end of the string
            // without a '=' character.  Ignore it since otherwise we
            // will access invalid memory below.
            continue;

          switch (len)
            {
            case 6:
              if (memcmp (envline, "CHECK_", 6) == 0)
                s = &envline[7];
              break;
            case 8:
              if (!__builtin_expect (__libc_enable_secure, 0))
                {
                  if (memcmp (envline, "TOP_PAD_", 8) == 0)
                    __libc_mallopt (M_TOP_PAD, atoi (&envline[9]));
                  else if (memcmp (envline, "PERTURB_", 8) == 0)
                    __libc_mallopt (M_PERTURB, atoi (&envline[9]));
                }
              break;
            case 9:
              if (!__builtin_expect (__libc_enable_secure, 0))
                {
                  if (memcmp (envline, "MMAP_MAX_", 9) == 0)
                    __libc_mallopt (M_MMAP_MAX, atoi (&envline[10]));
                  else if (memcmp (envline, "ARENA_MAX", 9) == 0)
                    __libc_mallopt (M_ARENA_MAX, atoi (&envline[10]));
                }
              break;
            case 10:
              if (!__builtin_expect (__libc_enable_secure, 0))
                {
                  if (memcmp (envline, "ARENA_TEST", 10) == 0)
                    __libc_mallopt (M_ARENA_TEST, atoi (&envline[11]));
                }
              break;
            case 15:
              if (!__builtin_expect (__libc_enable_secure, 0))
                {
                  if (memcmp (envline, "TRIM_THRESHOLD_", 15) == 0)
                    __libc_mallopt (M_TRIM_THRESHOLD, atoi (&envline[16]));
                  else if (memcmp (envline, "MMAP_THRESHOLD_", 15) == 0)
                    __libc_mallopt (M_MMAP_THRESHOLD, atoi (&envline[16]));
                }
              break;
            default:
              break;
            }
        }
    }

  if (s && s[0])
    {
      __libc_mallopt (M_CHECK_ACTION, (int) (s[0] - '0'));
      if (check_action() != 0)
        __malloc_check_init ();
    }
  void (*hook) (void) = atomic_forced_read (__malloc_initialize_hook);
  if (hook != NULL)
    (*hook)();
  __malloc_initialized() = 1;
}

#ifndef NO_THREADS

/* The following two functions are registered via thread_atfork() to
   make sure that the mutexes remain in a consistent state in the
   fork()ed version of a thread.  Also adapt the malloc and free hooks
   temporarily, because the `atfork' handler mechanism may use
   malloc/free internally (e.g. in LinuxThreads). */

void ptmalloc_lock_all (void)
{
  mstate ar_ptr;

  if (__malloc_initialized() < 1)
    return;

  /* We do not acquire free_list_lock here because we completely
     reconstruct free_list in ptmalloc_unlock_all2.  */

  if (mutex_trylock (&list_lock))
    {
      if (thread_arena == ATFORK_ARENA_PTR)
        /* This is the same thread which already locks the global list.
           Just bump the counter.  */
        goto out;

      /* This thread has to wait its turn.  */
      (void) mutex_lock (&list_lock);
    }
  for (ar_ptr = &main_arena();; )
    {
      (void) mutex_lock (&ar_ptr->mutex);
      ar_ptr = ar_ptr->next;
      if (ar_ptr == &main_arena())
        break;
    }
  save_malloc_hook = __malloc_hook;
  save_free_hook = __free_hook;
  __malloc_hook = malloc_atfork;
  __free_hook = free_atfork;
  /* Only the current thread may perform malloc/free calls now.
     save_arena will be reattached to the current thread, in
     ptmalloc_lock_all, so save_arena->attached_threads is not
     updated.  */
  save_arena = thread_arena;
  thread_arena = static_cast<mstate>(ATFORK_ARENA_PTR);
out:
  ++atfork_recursive_cntr;
}

void ptmalloc_unlock_all (void)
{
  mstate ar_ptr;

  if (__malloc_initialized() < 1)
    return;

  if (--atfork_recursive_cntr != 0)
    return;

  /* Replace ATFORK_ARENA_PTR with save_arena.
     save_arena->attached_threads was not changed in ptmalloc_lock_all
     and is still correct.  */
  thread_arena = static_cast<mstate>(save_arena);
  __malloc_hook = save_malloc_hook;
  __free_hook = save_free_hook;
  for (ar_ptr = &main_arena();; )
    {
      (void) mutex_unlock (&ar_ptr->mutex);
      ar_ptr = ar_ptr->next;
      if (ar_ptr == &main_arena())
        break;
    }
  (void) mutex_unlock (&list_lock);
}

# ifdef __linux__

/* In NPTL, unlocking a mutex in the child process after a
   fork() is currently unsafe, whereas re-initializing it is safe and
   does not leak resources.  Therefore, a special atfork handler is
   installed for the child. */

void ptmalloc_unlock_all2 (void)
{
  mstate ar_ptr;

  if (__malloc_initialized() < 1)
    return;

  thread_arena = static_cast<mstate>(save_arena);
  __malloc_hook = save_malloc_hook;
  __free_hook = save_free_hook;

  /* Push all arenas to the free list, except save_arena, which is
     attached to the current thread.  */
  mutex_init (&free_list_lock);
  if (save_arena != NULL)
    ((mstate) save_arena)->attached_threads = 1;
  free_list = NULL;
  for (ar_ptr = &main_arena();; )
    {
      mutex_init (&ar_ptr->mutex);
      if (ar_ptr != save_arena)
        {
	  /* This arena is no longer attached to any thread.  */
	  ar_ptr->attached_threads = 0;
          ar_ptr->next_free = free_list;
          free_list = ar_ptr;
        }
      ar_ptr = ar_ptr->next;
      if (ar_ptr == &main_arena())
        break;
    }

  mutex_init (&list_lock);
  atfork_recursive_cntr = 0;
}

# else

#  define ptmalloc_unlock_all2 ptmalloc_unlock_all

# endif

#endif  /* !NO_THREADS */

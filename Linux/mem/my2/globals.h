#pragma once

#include "types/malloc_state.h"
#include "types/malloc_param.h"
#include "fork.h"
#include "tls.h"

// This file declares all the global/singleton objects.

malloc_state & main_arena();

/* There is only one instance of the malloc parameters.  */

malloc_par & mp_();

/* Already initialized? */
int & __malloc_initialized();

/* Thread specific data.  */
extern __thread mstate thread_arena attribute_tls_model_ie;

fork_handler & atfork_mem();

extern int __libc_enable_secure;

int & check_action();

/* list_lock prevents concurrent writes to the next member of struct
   malloc_state objects.

   Read access to the next member is supposed to synchronize with the
   atomic_write_barrier and the write to the next member in
   _int_new_arena.  This suffers from data races; see the FIXME
   comments in _int_new_arena and reused_arena.

   list_lock also prevents concurrent forks.  At the time list_lock is
   acquired, no arena lock must have been acquired, but it is
   permitted to acquire arena locks subsequently, while list_lock is
   acquired.  */
extern mutex_t list_lock;

/* Counter for number of times the list is locked by the same thread.  */
extern unsigned int atfork_recursive_cntr;

/* Arena free list.  free_list_lock synchronizes access to the
   free_list variable below, and the next_free and attached_threads
   members of struct malloc_state objects.  No other locks must be
   acquired after free_list_lock has been acquired.  */

extern mutex_t free_list_lock;
extern size_t narenas;
extern mstate free_list;

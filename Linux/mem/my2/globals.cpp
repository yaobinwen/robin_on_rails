#include "globals.h"
#include "types/basic.h"
#include "types/malloc_state.h"
#include "types/malloc_param.h"
#include "types/top_pad.h"
#include "macros.h"
#include "mmap.h"

malloc_state & main_arena() {
    /*
        There are several instances of this struct ("arenas") in this
        malloc.  If you are adapting this malloc in a way that does NOT use
        a static or mmapped malloc_state, you MUST explicitly zero-fill it
        before using. This malloc relies on the property that malloc_state
        is initialized to all zeroes (as is true of C statics).
    */

    static struct malloc_state main_arena;
    static bool _initialized = false;
    if (!_initialized) {
        main_arena.mutex = _LIBC_LOCK_INITIALIZER;
        main_arena.next = &main_arena;
        main_arena.attached_threads = 1;
        _initialized = true;
    }

    return main_arena;
}

malloc_par & mp_() {
    static struct malloc_par p;
    static bool _initialized = false;
    if (!_initialized) {
        p.top_pad = DEFAULT_TOP_PAD;
        p.n_mmaps_max = DEFAULT_MMAP_MAX;
        p.mmap_threshold = DEFAULT_MMAP_THRESHOLD;
        p.trim_threshold = DEFAULT_TRIM_THRESHOLD;
        p.arena_test = NARENAS_FROM_NCORES (1);
        _initialized = true;
    }
    return p;
}

int & __malloc_initialized() {
    static int i = -1;
    return i;
}

__thread mstate thread_arena attribute_tls_model_ie;

fork_handler & atfork_mem() {
    static struct fork_handler h;
    return h;
}

/* Safest assumption, if somehow the initializer isn't run.  */
int __libc_enable_secure = 1;

int & check_action() {
    static int a = DEFAULT_CHECK_ACTION;
    return a;
}

mutex_t list_lock = _LIBC_LOCK_INITIALIZER;

unsigned int atfork_recursive_cntr;

mutex_t free_list_lock = _LIBC_LOCK_INITIALIZER;
size_t narenas = 1;
mstate free_list;

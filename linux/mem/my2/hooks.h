#pragma once

#include <cstddef>

#include "macros.h"

# define __MALLOC_HOOK_VOLATILE volatile

/* Called once when malloc is initialized; redefining this variable in
   the application provides the preferred way to set up the hook
   pointers. */
extern void (*__MALLOC_HOOK_VOLATILE __malloc_initialize_hook) (void)
__MALLOC_DEPRECATED;
/* Hooks for debugging and user-defined versions. */
extern void (*__MALLOC_HOOK_VOLATILE __free_hook) (void *__ptr,
                                                   const void *)
__MALLOC_DEPRECATED;
extern void *(*__MALLOC_HOOK_VOLATILE __malloc_hook)(size_t __size,
                                                     const void *)
__MALLOC_DEPRECATED;
extern void *(*__MALLOC_HOOK_VOLATILE __realloc_hook)(void *__ptr,
                                                      size_t __size,
                                                      const void *)
__MALLOC_DEPRECATED;
extern void *(*__MALLOC_HOOK_VOLATILE __memalign_hook)(size_t __alignment,
                                                       size_t __size,
                                                       const void *)
__MALLOC_DEPRECATED;
extern void (*__MALLOC_HOOK_VOLATILE __after_morecore_hook) (void);

int top_check (void);
void * malloc_check (size_t sz, const void *caller);
void * mem2mem_check (void *ptr, size_t req_sz);
void free_check (void *mem, const void *caller);
void * realloc_check (void *oldmem, size_t bytes, const void *caller);
void * memalign_check (size_t alignment, size_t bytes, const void *caller);

#pragma once

#ifndef atomic_forced_read
# define atomic_forced_read(x) \
  ({ __typeof (x) __x; __asm ("" : "=r" (__x) : "0" (x)); __x; })
#endif

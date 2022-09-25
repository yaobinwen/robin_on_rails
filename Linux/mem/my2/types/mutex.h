#pragma once

#include <pthread.h>

#define _LIBC_LOCK_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define mutex_t pthread_mutex_t

#define mutex_init(m) (pthread_mutex_init(m, nullptr))
#define mutex_lock pthread_mutex_lock
#define mutex_trylock pthread_mutex_trylock
#define mutex_unlock pthread_mutex_unlock

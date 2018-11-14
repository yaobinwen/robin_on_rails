#include "HeapMgr.h"
#include "MallocState.h"
#include "Arena.h"
#include "TLS.h"

void * HeapMgr::Alloc(size_t bytes) {
    MStatePtr ar_ptr;
    void * victim;

    // arena_get (ar_ptr, bytes);
    {
        ar_ptr = TLS::Instance().GetArena();
        if (ar_ptr != NULL && !ArenaValidator::IsCorrupt(ar_ptr)) {
            ar_ptr->mutex.Lock();
        } else {
            // ptr = arena_get2 ((size), NULL);
            // TODO: Implement me!
        }
    }
}

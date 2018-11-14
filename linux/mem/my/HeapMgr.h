#pragma once

#include <cstddef>  // for size_t

// The heap manager that allocates and frees memory.
class HeapMgr
{
public:
    void * Alloc(size_t bytes);
    void Free(void * mem);
};

#include "TLS.h"

/* static */
TLS & TLS::Instance()
{
    static TLS _inst;
    return _inst;
}

ArenaPtr TLS::GetArena() const
{
    // TODO(ywen): Implement me!
    return &Arena();
}

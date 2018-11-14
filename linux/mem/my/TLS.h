#pragma once

#include "Arena.h"

// Thread-specific storage.
class TLS
{
public:
    static TLS & Instance();

public:
    ArenaPtr GetArena() const;
};

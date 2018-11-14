#pragma once

#include "MallocState.h"

typedef MallocState Arena;
typedef MStatePtr ArenaPtr;

namespace ArenaValidator
{

bool IsCorrupt(ArenaPtr ptr);

};  // end of namespace ArenaValidator

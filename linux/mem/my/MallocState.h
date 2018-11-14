#pragma once

#include "Mutex.h"

struct MallocState
{

    Mutex mutex;

};

typedef MallocState * MStatePtr;

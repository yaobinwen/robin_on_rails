#include "conv_pid.h"
#include <stdlib.h>
#include <string.h>

int conv_pid(char * pid_str, unsigned int *result)
{
    char * end = NULL;
    unsigned long pid = strtoul(pid_str, &end, 10);
    if (end)
    {
        /* `pid_str` is not purely numeric. */
        return 1;
    }

    *result = pid;
    return 0;
}

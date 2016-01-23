#include "nyx_shared.h"
#include "hash.h"

#define HASH_MULT 65599
unsigned hashstr(const wchar_t *str)
{
    unsigned hash = 0;

    while (*str) {
        hash = hash * HASH_MULT + *str;
        str++;
    }

    return hash;
}

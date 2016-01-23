/*
 * FIXME: Realised this is bollocks. The table needs to be an array, really.
 */

#include "nyx_shared.h"
#include "htable.h"
#include "error.h"
#include "hash.h"

#define NAME_CHARS 128

struct htable_entry {
    wchar_t name[NAME_CHARS];
    void *data;

    struct htable_entry *next;
};

static struct htable_entry *test_table[383] = { NULL };

static nboolean is_prime(size_t n)
{
    size_t i;

    for (i = 2; i <= n / 2; i++) {
        if (n % i == 0) return nfalse;
    }

    return ntrue;
}

#if 0
static size_t next_prime(size_t n)
{
    for ( ; n < n*n; n++) {
        if (is_prime(n)) return n;
    }

    /* not reached */
    return 1;
}
#endif

void htable_add(struct htable_entry **table, size_t tsz,
                const wchar_t *key, void *value)
{

}


void test_htable()
{

}

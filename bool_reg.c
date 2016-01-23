/*
 * Boolean registry.
 * A mapping of strings to boolean values, used to store game logic flags.
 *
 * 2014-07-07 -- first draft
 *  Going to start with a sorted array we can binary search, before
 *  jumping the gun and going for a hash table or something equally monstrous.
 *  The array only needs sorting once when everything has loaded in, and 
 *  lookups should be relatively infrequent so a binary search should suffice.
 *
 */
#include "nyx_shared.h"
#include "error.h"

#define ENTRY_STR_SZ 128

struct entry {
    wchar_t str[ENTRY_STR_SZ];
    nboolean value;
};

static struct entry *registry[1024] = { NULL };
static size_t next_idx = 0;

static struct entry *create_entry(wchar_t *str, nboolean value)
{
    struct entry *e = NULL;

    if (!str) {
        ERR("received NULL string");
    }

    e = malloc(sizeof(*e));
    if (!e) {
        ERR("failed to allocate memory");
    }

    if (wcslen(str) >= ENTRY_STR_SZ) {
        free(e);
        ERR("string too long");
    }

    e->value = value;
    wcsncpy(e->str, str, ENTRY_STR_SZ);

    return e;
}

static void destroy_registry()
{
    size_t i;

    for (i = 0; i < next_idx; i++) {
        free(registry[i]);
    }
}

static int compare(const void *a, const void *b)
{
    struct entry *entrya = *(struct entry **) a;
    struct entry *entryb = *(struct entry **) b;

    return wcsicmp(entrya->str, entryb->str);
}

static void sort_registry()
{
    qsort((void *) registry, next_idx, sizeof(struct entry *), compare);
}

static void dump_registry()
{
    size_t i;

    for (i = 0; i < next_idx; i++) {
        wprintf(L"Entry %u: %s / %d\n", i, registry[i]->str, registry[i]->value);
    }
}

void add_entry(wchar_t *str, nboolean value)
{
    struct entry *e = NULL;

    e = create_entry(str, value);
    wprintf(L"Adding entry '%s' (%d) at index %u\n", str, value, next_idx);
    registry[next_idx++] = e;
}

nboolean find_entry(wchar_t *str)
{
    struct entry *e = NULL;
    size_t i;
    size_t len;

    if (!str) {
        ERR("received NULL string");
    }

    len = wcslen(str);
    for (i = 0; i < next_idx; i++) {
        if (wcsncmp(registry[i]->str, str, len) == 0) {
            return registry[i]->value;
        }
    }

    ERR("entry not found");
}


void test_bool_reg()
{
    add_entry(L"met-lee", ntrue);
    add_entry(L"raped-a-camel", nfalse);
    add_entry(L"penetrated-a-lady", ntrue);
    add_entry(L"murdered-alistair", nfalse);
    add_entry(L"eaten-an-aardvark", nfalse);

    sort_registry();

    {
        nboolean v = find_entry(L"met-lee");
        wprintf(L"v = %d\n", v);
    }

    destroy_registry();
}
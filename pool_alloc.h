/*
 * Pool memory allocator.
 */
#pragma once

struct mempool_s;
typedef struct mempool_s mempool_t;
typedef size_t poolhandle_t;

/* create an empty mempool. dbg_name can be NULL */
mempool_t *create_mempool(size_t blocksize, size_t maxblocks, str_t *dbg_name);
void destroy_mempool(mempool_t *pool);

/* return a handle to the next free block in the given pool */
poolhandle_t pool_alloc(mempool_t *pool);

/* return a pointer to the block at the given handle */
void *pool_pointer(mempool_t *pool, poolhandle_t h);

/* mark the block at the given handle as free */
void pool_free(mempool_t *pool, poolhandle_t h);

/* debugging */
typedef void (*pool_itercb)(void *data, size_t blocknum, bool_t free);
void pool_iterate(mempool_t *pool, pool_itercb callback);
#include "tb_shared.h"
#include "pool_alloc.h"

typedef struct pool_block_s {
    void *data;
    bool_t free;

    struct pool_block_s *next;
} pool_block_t;

struct mempool_s {
    str_t *debug_name;  /* optionally set by create_mempool() for debugging */
    size_t blocksize;
    size_t numblocks;

    pool_block_t *blocks;
};

mempool_t *create_mempool(size_t blocksize, size_t numblocks, str_t *dbg_name)
{
    mempool_t *pool = NULL;
    size_t blocks = 0;

    if (blocksize < 1)
        panic("create_mempool: blocksize < 1");

    if (numblocks < 1)
        panic("create_mempool: numblocks < 1");

    pool = tb_malloc(sizeof(*pool));
    pool->blocksize = blocksize;
    pool->numblocks = numblocks;

    if (dbg_name)
        pool->debug_name = dbg_name;

    while (blocks < numblocks) {
        pool_block_t *block = tb_malloc(sizeof(*block));

        block->free = TRUE;
        block->data = tb_malloc(blocksize);
        block->next = pool->blocks;
        pool->blocks = block;

        blocks++;
    }

    plog("created pool: '%s', %u blocks of %u bytes", dbg_name, numblocks, blocksize);

    return pool;
}

void destroy_mempool(mempool_t *pool)
{
    pool_block_t *iter, *dead;
    size_t deadcount = 0;

    if (!pool)
        panic("destroy_mempool: NULL pool");

    for (iter = pool->blocks; iter; ) {
        dead = iter;
        iter = iter->next;

        tb_free(dead->data);
        tb_free(dead);
        deadcount++;
    }

    if (deadcount != pool->numblocks)
        panic("destroy_mempool: shit son");

    plog("destroyed pool: '%s', %u blocks of %u bytes", pool->debug_name, deadcount, pool->blocksize);
    tb_free(pool);
}

/*
 * Allocate from given pool.
 * Returns a handle to be turned into a pointer with pool_pointer()
 */
poolhandle_t pool_alloc(mempool_t *pool)
{
    poolhandle_t handle = 0;
    pool_block_t *iter = NULL;

    if (!pool)
        panic("pool_alloc: NULL pool");

    for (iter = pool->blocks; iter; iter = iter->next, handle++) {
        if (iter->free) {
            iter->free = FALSE;
            return handle;
        }
    }

    panic("pool_alloc: no free blocks");
    return 0; /* not reached */
}

void *pool_pointer(mempool_t *pool, poolhandle_t h)
{
    pool_block_t *iter = NULL;
    poolhandle_t dst;

    if (!pool)
        panic("pool_pointer: NULL pool");

    if (h > pool->numblocks)
        panic("pool_pointer: invalid handle");

    for (iter = pool->blocks, dst = 0; dst != h; dst++) {
        iter = iter->next;
    }

    return iter->data;
}

void pool_free(mempool_t *pool, poolhandle_t h)
{
    pool_block_t *iter = NULL;
    poolhandle_t dst;

    if (!pool)
        panic("pool_free: NULL pool");

    if (h > pool->numblocks)
        panic("pool_free: invalid handle");

    for (iter = pool->blocks, dst = 0; dst != h; dst++) {
        iter = iter->next;
    }

    iter->free = TRUE;
    memset(iter->data, 0, pool->blocksize);
}

void pool_iterate(mempool_t *pool, pool_itercb callback)
{
    pool_block_t *iter = NULL;
    size_t num = 0;

    if (!pool)
        panic("pool_iterate: NULL pool");

    if (!callback)
        panic("pool_iterate: NULL callback");

    for (iter = pool->blocks; iter; iter = iter->next, num++)
        callback(iter->data, num, iter->free);
}
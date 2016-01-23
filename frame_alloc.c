#include "tb_shared.h"
#include "frame_alloc.h"

static byte *buffer = NULL;
static size_t buffersz = 0;
static size_t mark = 0;

void frame_alloc_create(size_t poolsz)
{
    if (buffer != NULL)
        return;

    if (poolsz < 1)
        panic("frame_alloc_create: invalid poolsz");

    buffer = tb_malloc(poolsz);
    buffersz = poolsz;
    mark = 0;
}

void frame_alloc_destroy()
{
    tb_free(buffer);
    buffer = NULL;
    mark = 0;
}

void *frame_alloc(size_t bytes)
{
    void *ret = (void *) buffer[mark];

    if (bytes < 1)
        panic("frame_alloc: zero allocation");

    if (bytes > buffersz || (mark + bytes) > buffersz)
        panic("frame_alloc: failed on allocation of %u bytes", bytes);

    mark += bytes;
    return ret;
}

void frame_alloc_reset()
{
    mark = 0;
}
#include "tb_shared.h"
#include "fsys.h"
#include "commands.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>

#define MAX_FNAME_CHARS 256
#define MAX_FILE_HANDLES 128

typedef struct filedata_s {
    str_t name[MAX_FNAME_CHARS];
    size_t size;
    FILE *fhandle;
} filedata_t;

static filedata_t files[MAX_FILE_HANDLES];

static void clear_handles()
{
    memset(files, 0, sizeof(files[0]) * MAX_FILE_HANDLES);
}

static filehandle_t next_free_handle()
{
    filehandle_t h;

    for (h = 0; h < MAX_FILE_HANDLES; h++) {
        if (files[h].fhandle == NULL)
            return h;
    }

    panic("next_free_handle: no free handles");
    return 0; /* not reached */
}

static size_t file_size(const str_t *path)
{
    struct _stat stat;
    int fd;

    if ((fd = _open(path, _O_RDONLY)) == -1)
        panic("file_size: failed to open %s", path);

    _fstat(fd, &stat);
    _close(fd);

    return (size_t) stat.st_size;
}

void fsys_init()
{
    clear_handles();
}

void fsys_shutdown()
{
    size_t i;

    for (i = 0; i < MAX_FILE_HANDLES; i++) {
        if (files[i].fhandle != NULL)
            fclose(files[i].fhandle);
    }

    clear_handles();
}

filehandle_t fsys_open(const str_t *path)
{
    filehandle_t ret;
    filedata_t *data = NULL;

    if (!path)
        panic("fsys_open: NULL path");

    if (strlen(path) >= MAX_FNAME_CHARS)
        panic("fsys_open: path too long");

    ret = next_free_handle();
    data = &files[ret];

    tb_strncpyz(path, data->name, MAX_FNAME_CHARS);
    data->size = file_size(path);
    data->fhandle = fopen(path, "rb");

    /* this "shouldn't" happen because file_size() just opened it... */
    if (!data->fhandle)
        panic("fsys_open: failed to open %s", path);

    return ret;
}

void fsys_close(filehandle_t h)
{
    if (h > MAX_FILE_HANDLES)
        panic("fsys_close: invalid handle");

    if (files[h].fhandle == NULL)
        panic("fsys_close: handle already closed");

    fclose(files[h].fhandle);
    files[h].size = 0;
    memset(files[h].name, 0, MAX_FNAME_CHARS);
}

byte *fsys_get_data(filehandle_t h)
{
    byte *buffer = NULL;

    if (h > MAX_FILE_HANDLES)
        panic("fsys_get_data: invalid handle");

    if (files[h].fhandle == NULL)
        panic("fsys_get_data: file for handle %u not open", h);

    buffer = tb_malloc(files[h].size * sizeof(*buffer) + 1);
    fread(buffer, sizeof(*buffer), files[h].size, files[h].fhandle);
    buffer[files[h].size] = 0;

    return buffer;
}

size_t fsys_get_size(filehandle_t h)
{
    if (h > MAX_FILE_HANDLES)
        panic("fsys_get_size: invalid handle");

    if (files[h].fhandle == NULL)
        panic("fsys_get_size: file for handle %u not open", h);

    return files[h].size;
}
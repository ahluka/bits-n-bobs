/*
 * Support functions used in many places. Bit of a kitchen sink.
 */
#include "tb_shared.h"

#define LOGFILE_NAME "log.txt"
static FILE *logfile = NULL;

static void close_logfile(void)
{
    fclose(logfile);
}

static void open_logfile()
{
    if (!logfile) {
        logfile = fopen(LOGFILE_NAME, "w");
        if (!logfile) {
            fprintf(stderr, "Failed to open log file at %s\n", LOGFILE_NAME);
            exit(1);
        }

        atexit(close_logfile);
    }
}

/*
 * varargs printf into a buffer so we don't need to have
 * varargs functions all over the place.
 */
str_t *vfmt(str_t *format, ...)
{
    va_list args;
    static str_t buffer[2][16000];
    static int index = 0;
    str_t *buf;

    buf = buffer[index & 1];
    index++;

    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    return buf;
}

/*
 * Something has gone wrong, dump error message and exit().
 */
void panic(str_t *format, ...)
{
    va_list args;
    str_t buffer[1024];

    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);

    open_logfile();
    fprintf(logfile, "[PANIC] %s\n", buffer);
    fprintf(stderr, "[PANIC] %s\n", buffer);

    exit(1);
}

/*
 * Write given message to the log file.
 */
void plog(str_t *format, ...)
{
    va_list args;
    str_t buffer[1024];

    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);

    open_logfile();
    fprintf(logfile, "[log] %s\n", buffer);
    printf("[log] %s\n", buffer);
}

/*
 * Safe strncpy() that ensures trailing zero.
 */
void tb_strncpyz(const str_t *src, str_t *dest, size_t destsz)
{
    if (!src)
        panic("tb_strncpyz: NULL src");

    if (!dest)
        panic("tb_strncpyz: NULL dest");

    if (destsz < 1)
        panic("tb_strncpyz: destsz < 1");

    strncpy(dest, src, destsz - 1);
    dest[destsz - 1] = 0;
}

/*
 * Safe strncat() that ensures trailing zero.
 */
void tb_strncat(const str_t *src, str_t *dest, size_t destsz)
{
    size_t len;

    if (!src)
        panic("tb_strncat: NULL src");

    if (!dest)
        panic("tb_strncat: NULL dest");

    if (destsz < 1)
        panic("tb_strncat: destsz < 1");

    len = strlen(dest);
    if (len >= destsz)
        panic("tb_strncat: overflow");

    tb_strncpyz(src, dest + len, destsz - len);
}

/*
 * Safe malloc() that always zeros the allocated buffer.
 */
void *tb_malloc(size_t sz)
{
    void *block = NULL;

    if (!sz)
        panic("tb_malloc: allocation of zero bytes");

    block = malloc(sz);
    if (!block)
        panic("tb_malloc: failed on allocation of %u bytes", sz);

    memset(block, 0, sz);
    return block;
}

/*
 * Just a wrapper for free() for now.
 * Maybe add some memory stats in the future?
 */
void tb_free(void *block)
{
    if (!block)
        panic("tb_free: NULL block");

    free(block);
}
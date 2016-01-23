/*
 * tb_shared.h -- included first by all modules.
 */
#pragma once

#define TB_VERSION "0.1a"

/* WIN32 specifics; headers, compiler switches, etc */
#ifdef _WIN32

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#endif /* _WIN32 */

/* Standard headers */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

/* SDL */
#include <SDL\SDL.h>
#include <SDL\SDL_image.h>
#include <SDL\SDL_ttf.h>
#include <SDL\SDL_mixer.h>

/* Platform-agnostic definitions */
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif
typedef enum {FALSE, TRUE} bool_t;

typedef unsigned char byte;
typedef char str_t;

/* Widely used declarations */
void panic(str_t *format, ...);
void plog(str_t *format, ...);

str_t *vfmt(str_t *format, ...);
void tb_strncpyz(const str_t *src, str_t *dest, size_t destsz);
void tb_strncat(const str_t *src, str_t *dest, size_t destsz);

void *tb_malloc(size_t sz);
void tb_free(void *block);
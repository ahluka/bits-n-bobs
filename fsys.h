/*
 * File system.
 */
#pragma once

typedef unsigned filehandle_t;

void fsys_init();
void fsys_shutdown();

/* open the specified file and return a handle to it */
filehandle_t fsys_open(const str_t *path);

/* close the specified file handle */
void fsys_close(filehandle_t h);

/* get the contents of the file specified by the given handle */
/* remember to free to the returned buffer! */
byte *fsys_get_data(filehandle_t h);

/* get the size (in bytes) of the file specified by the given handle */
size_t fsys_get_size(filehandle_t h);
/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; of version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TVU_BUFFER_H
#define TVU_BUFFER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>

#define TVU_BUFFER_ERROR    -1
#define TVU_BUFFER_UNKNOWN  -2
#define TVU_BUFFER_MALLOC   -3
#define TVU_BUFFER_SHM      -4

/** Buffer.
 */
typedef struct {
    uint8_t *data;      //< Data buffer.
    size_t  size;       //< Size of data.
    off_t   offset;     //< User defined offset inside the data.
    int     fd;         //< >0 file descriptor, -1 error, -2 malloc, -3 shared-mem, -4 own management.
} tvu_buffer_t;

/** Allocate a buffer.
 * @param size      Size of the buffer to allocate.
 * @returns         Buffer with allocated memory.
 */
static inline tvu_buffer_t tvu_buffer_alloc(size_t size)
{
    tvu_buffer_t buffer = {
        .data   = malloc(size),
        .size   = size,
        .offset = 0,
        .fd     = TVU_BUFFER_MALLOC
    };

    if (buffer.data == NULL) {
        abort();
    }

    return buffer;
}

/** Create an error.
 * @returns         Error buffer.
 */
static inline tvu_buffer_t tvu_buffer_error(void)
{
    tvu_buffer_t buffer = {
        .data = NULL,
        .size = 0,
        .offset = 0,
        .fd = TVU_BUFFER_ERROR
    };
    return buffer;
}

static inline bool tvu_buffer_is_error(tvu_buffer_t buffer)
{
    return buffer.fd == TVU_BUFFER_ERROR;
}

static inline void tvu_buffer_free(tvu_buffer_t buffer)
{
    switch (buffer.fd) {
    case TVU_BUFFER_ERROR:
        break;
    case TVU_BUFFER_UNKNOWN:
        break;
    case TVU_BUFFER_MALLOC:
        free(buffer.data);
        break;
    case TVU_BUFFER_SHM:
        shmdt(buffer.data);
        break;
    default:    // mmap
        munmap(buffer.data, buffer.size);
        close(buffer.fd);
    }
}

#endif

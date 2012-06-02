#ifndef TVU_BUFFER_H
#define TVU_BUFFER_H

#include <stdint.h>
#include <unistd.h>

typedef struct {
    uint8_t *data;
    size_t  size;
    off_t   offset;
} tvu_buffer_t;

#endif

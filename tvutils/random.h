/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TVU_RANDOM_H
#define TVU_RANDOM_H

#include <tvutils/types.h>
#include <stdint.h>
#include <xmmintrin.h>

#define TVU_RANDOM_DATA_SIZE        0x10000
#define TVU_RANDOM_DATA_BYTE_SIZE   (TVU_RANDOM_DATA_SIZE * sizeof (uint64_t))
extern __thread uint64_t    tvu_random_data[TVU_RANDOM_DATA_SIZE];
extern __thread uint64_t    tvu_random_count;

void tvu_random_load(void);


#ifdef __RDRND__
static inline uint64_t tvu_random(void)
{
    uint64_t r;
    __builtin_ia32_rdrand64_step(&r);
    return r;
}
#endif

#ifndef __RDRND__
static inline uint64_t tvu_random(void)
{
    tvu_int i = tvu_random_count++ % TVU_RANDOM_DATA_SIZE;

    if (i == 0) {
        tvu_random_load();
    }

    return tvu_random_data[i];
}
#endif

static inline int64x2_t tvu_random128(void)
{
    int64x2_t r = {tvu_random(), tvu_random()};
    return r;
}

#endif

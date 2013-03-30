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
#ifndef TVU_TYPES_H
#define TVU_TYPES_H

#include <stdint.h>

typedef unsigned int    uint128_t __attribute__((mode(TI)));
typedef int             int128_t __attribute__((mode(TI)));

typedef float           float32_t;
typedef double          float64_t;
typedef long double     float80_t;

/** Standard float type.
 * Use this type anywhere you need a non specific sized float.
 */
typedef float64_t       tvu_float;

/** Standard int type.
 * Use this type anywhere you need a non specific sized int.
 */
typedef int64_t         tvu_int;

/** Standard UTF-t string type.
 */
typedef char            utf8_t;

/** Result of iterative find function.
 */
typedef enum {
    TVU_FIND_NEXT,
    TVU_FIND_FOUND,
    TVU_FIND_END
} tvu_find_t;

typedef int8_t  int1x16_t __attribute__((__vector_size__(16)));
typedef int16_t int16x8_t __attribute__((__vector_size__(16)));
typedef int32_t int32x4_t __attribute__((__vector_size__(16)));
typedef int64_t int64x2_t __attribute__((__vector_size__(16)));

typedef struct {
    int64x2_t   v[8];
} int64x2x8_t;

typedef struct {
    int64x2_t   v[11];
} int64x2x11_t;

typedef struct {
    uint64_t   v[32];
} uint2048_t;

#endif

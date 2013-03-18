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
#ifndef TVU_TIME_POSIX1993_H
#define TVU_TIME_POSIX1993_H

#ifndef TVU_TIME_H_IN
#error "Do not manually include this file, it should be included only from tvutils/time.h"
#endif

#if _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif
#include <time.h>

/** Get the time.
 * The time returned by this function is a fixed point number.
 * The upper 32 bits are the number of seconds since 2012-01-01 UTC.
 * The lower 32 bits are the fraction of seconds.
 * Negative values are currently not supported.
 *
 * @returns The time.
 */
static inline tvu_time_t tvu_time(void)
{
    struct timespec ts;
    tvu_time_t      t;
    uint64_t        frac_of_seconds;

    clock_gettime(CLOCK_REALTIME, &ts);

    t  = ((uint64_t)ts.tv_sec  - TVU_TIME_EPOCH) << 32;
    t |= ((uint64_t)ts.tv_nsec * TVU_NS_TO_FRAC) >> 32;
    t += 2; // Add half a nanosecond.
    return t;
}

#endif

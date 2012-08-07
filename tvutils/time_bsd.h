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
#ifndef TVU_TIME_BSD_H
#define TVU_TIME_BSD_H

#ifndef TVU_TIME_H_IN
#error "Do not manually include this file, it should be included only from tvutils/time.h"
#endif

#include <sys/time.h>

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
    struct timeval  ts;
    tvu_time_t      t;
    uint64_t        frac_of_seconds;

    gettimeofday(&ts, NULL);

    t  = ((uint64_t)ts.tv_sec  - TVU_TIME_EPOCH) << 32;
    t |= ((uint64_t)ts.tv_usec * TVU_US_TO_FRAC) >> 32;
    t += 2;     // Add half a nanosecond.
    return t;
}

#endif

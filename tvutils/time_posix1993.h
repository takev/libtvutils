/* libtvutils - Library with useful utilities.
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
#ifndef TVU_TIME_POSIX1993_H
#define TVU_TIME_POSIX1993_H

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
    return t;
}

#endif

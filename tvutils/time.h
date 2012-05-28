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
#ifndef TVU_TIME_H
#define TVU_TIME_H
#define TVU_TIME_H_IN

#include <stdlib.h>
#include <stdint.h>

// 2012-01-01 with reference 1970-01-01 in seconds.
#define TVU_TIME_EPOCH     1325376000ULL
#define TVU_NS_TO_FRAC     18446744074ULL
#define TVU_US_TO_FRAC     18446744073710ULL

/** The time.
 * A 64 bit number as the number of 1/(2^32) seconds since January 1st, 2012.
 * You can also see this as a 32.32 fixed point number, representing the
 * number of seconds since January 1st, 2012.
 */
typedef int64_t tvu_time_t;

#if __GNUC_LIBRARY__ >= 6
#include <tvutils/time_posix1993.h>

#elif defined(__APPLE__)
#include <tvutils/time_bsd.h>

#else
#error "Could not find a time implemtation for this platfor."
#endif

#undef TVU_TIME_H_IN
#endif

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
#ifndef TVU_TIME_H
#define TVU_TIME_H
#define TVU_TIME_H_IN

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <tvutils/types.h>
#include <tvutils/target.h>

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

#ifndef TVU_HAVE_TIMESPEC
typedef struct timespec {
    tvu_int     tv_sec;
    tvu_int     tv_nsec;
};
#endif

#ifndef TVU_HAVE_TIMEVAL
typedef struct timeval {
    tvu_int     tv_sec;
    tvu_int     tv_usec;
};
#endif

static inline struct timespec tvu_to_timespec(tvu_time_t t)
{
    struct timespec ts = {
        .tv_sec = t >> 32,
        .tv_nsec = (t << 32) / TVU_NS_TO_FRAC
    };
    return ts;
}

static inline struct timeval tvu_to_timeval(tvu_time_t t)
{
    struct timeval ts = {
        .tv_sec = t >> 32,
        .tv_usec = ((t << 32) / TVU_NS_TO_FRAC) / 1000
    };
    return ts;
}

static inline tvu_int tvu_to_ns(tvu_time_t t)
{
    struct timespec ts = tvu_to_timespec(t);
    return (tvu_int)ts.tv_nsec + ((tvu_int)ts.tv_sec * 1000000000LL);
}

static inline tvu_int tvu_to_us(tvu_time_t t)
{
    struct timeval ts = tvu_to_timeval(t);
    return (tvu_int)ts.tv_usec + ((tvu_int)ts.tv_sec * 1000000LL);
}

static inline tvu_int tvu_to_ms(tvu_time_t t)
{
    struct timeval ts = tvu_to_timeval(t);
    return ((tvu_int)ts.tv_usec / 1000LL) + ((tvu_int)ts.tv_sec * 1000LL);
}

/** Number of hours since 2012-01-01.
 * @param t     Timestamp.
 * @returns     Number of hours since 2012-01-01.
 */
static inline tvu_int tvu_to_hours(tvu_time_t t)
{
    return (t >> 32) / 3600;
}

/** Convert timestamp to time_t.
 */
static inline time_t tvu_to_timet(tvu_time_t t)
{
    tvu_int seconds_since_2012 = t >> 32;
    tvu_int seconds_since_1970 = seconds_since_2012 + TVU_TIME_EPOCH;
    return seconds_since_1970;
}

#include <stdio.h>

/** Convert timestamp into a time tuple structure.
 */
static inline struct tm tvu_to_tm_utc(tvu_time_t t)
{
    time_t      tt = tvu_to_timet(t);
    struct tm   ttuple;

    gmtime_r(&tt, &ttuple);
    return ttuple;
}

/** Format a date time.
 * Format a timestamp at utc using strftime semantics. As extra you may use %N at the
 * end of the format to add 9 digit nanoseconds to the string.
 *
 * @param s         Output string.
 * @param maxsize   Size of output string.
 * @param _format   Format string.
 * @param t         Timestamp.
 */
tvu_int tvu_strftime_utc(utf8_t * restrict s, tvu_int maxsize, const utf8_t *restrict _format, tvu_time_t t);

/** Format a date.
 * Format a string like this: YYYY-MM-DD
 * This function adds a nul terminating character.
 *
 * @param s     pointer to a string of 11 bytes.
 * @param t     timestamp
 */
static inline tvu_int tvu_fmt_date(utf8_t * restrict s, tvu_time_t t)
{
    return tvu_strftime_utc(s, 11, "%Y-%m-%d", t);
}

/** Format a short date.
 * Format a string like this: YYYYMMDD
 * This function adds a nul terminating character.
 *
 * @param s     pointer to a string of 9 bytes.
 * @param t     timestamp
 */
static inline tvu_int tvu_fmt_shortdate(utf8_t * restrict s, tvu_time_t t)
{
    return tvu_strftime_utc(s, 9, "%Y%m%d", t);
}

/** Format a time.
 * Format a string like this: HH:MM:SS
 * This function adds a nul terminating character.
 *
 * @param s     pointer to a string of 9 bytes.
 * @param t     timestamp
 */
static inline tvu_int tvu_fmt_time(utf8_t * restrict s, tvu_time_t t)
{
    return tvu_strftime_utc(s, 9, "%H:%M:%S", t);
}

/** Format a date time.
 * Format a string like this: YYYY-MM-DD HH:MM:SS
 * This function adds a nul terminating character.
 *
 * @param s     pointer to a string of 20 bytes.
 * @param t     timestamp
 */
static inline tvu_int tvu_fmt_datetime(utf8_t * restrict s, tvu_time_t t)
{
    return tvu_strftime_utc(s, 20, "%Y-%m-%d %H:%M:%S", t);
}

/** Format a timestamp.
 * Format a string like this: YYYY-MM-DD HH:MM:SS.nnnnnnnnn
 * This function adds a nul terminating character.
 *
 * @param s     pointer to a string of 30 bytes.
 * @param t     timestamp
 */
static inline tvu_int tvu_fmt_timestamp(utf8_t * restrict s, tvu_time_t t)
{
    return tvu_strftime_utc(s, 30, "%Y-%m-%d %H:%M:%S.%N", t);
}

#if __GLIBC__ >= 2
#include <tvutils/time_posix1993.h>

#elif defined(__APPLE__)
#include <tvutils/time_bsd.h>

#else
#error "Could not find a time implemtation for this platfor."
#endif

#undef TVU_TIME_H_IN
#endif

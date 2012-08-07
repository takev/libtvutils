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
#include <stdbool.h>
#include <stdio.h>
#include <tvutils/types.h>
#include <tvutils/string.h>
#include <tvutils/time.h>

tvu_int tvu_strftime_utc(utf8_t * restrict s, tvu_int maxsize, const utf8_t *restrict _format, tvu_time_t t)
{
    tvu_timespec_t  ts = tvu_to_timespec(t);
    struct tm       time_tuple = tvu_to_tm_utc(t);
    tvu_int         format_len = strlen(_format);
    utf8_t          *format = tvu_strdupa(_format);
    bool            format_has_nanosecond;
    tvu_int         s_offset;

    // Check for trailing %N.
    if ((format_len >= 2) && (format[format_len - 2] == '%') && (format[format_len - 1] == 'N')) {
        format_has_nanosecond = true;
        format[format_len - 2] = 0; // Remove the %N from the format.
    } else {
        format_has_nanosecond = false;
    }
    
    if ((s_offset = strftime(s, maxsize, format, &time_tuple)) == 0) {
        s[0] = 0;
        return 0;
    }

    if (format_has_nanosecond) {
        // s_offset can never be larger than maxsize, so at least 0 bytes are left.
        s_offset += snprintf(&s[s_offset], maxsize - s_offset, "%09lli", ts.tv_nsec);
    }
    return s_offset;
}


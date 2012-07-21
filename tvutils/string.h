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
#ifndef TVU_STRING_H
#define TVU_STRING_H

#include <stdint.h>
#include <tvutils/types.h>

/** Count a character in a string.
 * @param haystack  The string to search in.
 * @param needle    The character to search for.
 * @returns         The number of times needle is found in haystack.
 */
static inline size_t tvu_count_character(utf8_t const * restrict haystack, utf8_t needle)
{
    size_t  count = 0;
    int     i = 0;
    utf8_t  c;

    while ((c = haystack[i++])) {
        count+= (c == needle);
    }

    return count;
}

/** Print error message.
 * Like the system perror() function, but with formatted string.
 * @param fmt   Formatted string.
 * @param ...   Parameters to format.
 */
void tvu_perror(utf8_t const * restrict fmt, ...) __attribute ((format (printf, 1, 0)));

#endif

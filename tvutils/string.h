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

typedef uint8_t utf8_t;

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

static inline size_t tvu_strlen(utf8_t const * restrict s)
{
    return strlen((char const *)s);
}

#endif

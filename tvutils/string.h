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
#include <string.h>
#include <alloca.h>
#include <stdlib.h>
#include <tvutils/types.h>
#include <tvutils/compiler.h>

/** String dupplicate with string on the stack.
 */
#define tvu_strdupa(src)    strcpy(alloca(strlen(src) + 1), src)

/** Copy n bytes from memory area s2 to memory area s1.
 * When s2 is not null, see behaviour of memcpy.
 * When s1 is null, see behavour of memset() with c = 0.
 *
 * @param s1    Destination memory area.
 * @param s2    Source memory area, this may be NULL unlike normal memcpy().
 * @param n     Number of bytes to copy.
 */
static inline void *tvu_memcpy_null(void * restrict s1, void const * restrict s2, size_t n)
{
    if (likely(s2 != NULL)) {
        return memcpy(s1, s2, n);
    } else {
        return memset(s1, 0, n);
    }
}

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

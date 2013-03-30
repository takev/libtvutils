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

/** Convert a Hexadecimal character into a nibble.
 * @param   c hexadecimal character.
 * @return  0-15 decoded character.
 *          -1 when no character decoded.
 *          -2 when null found.
 */
static inline tvu_int tvu_hex2nibble(utf8_t c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 0xa;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 0xA;
    } else if (c == 0) {
        return -2;
    } else {
        return -1;
    }
}

/** Convert a nibble to a hex character.
 * @param   x value between 0 to 15.
 * @return  A hex digit
 */
static inline tvu_int tvu_nibble2hex(utf8_t x)
{
    if (x <= 9) {
        return '0' + x;
    } else {
        return 'a' + x - 10;
    }
}


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

/** Display the data to a void pointer.
 * @param _src  Pointer to data.
 * @param src_size  Nr of byte of data.
 * @return      malloc-ed string.
 */
utf8_t *tvu_voidp2hex(void *_src, size_t src_size);

#endif

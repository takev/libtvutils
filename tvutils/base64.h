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
#ifndef TVU_BASE64_H
#define TVU_BASE64_H

#include <tvutils/types.h>

extern utf8_t tvu_base64_enc_table[];
extern utf8_t tvu_base64_dec_table[];

/** Encoding table according to RFC-1924
 */
extern utf8_t tvu_base85_enc_table[];

/** Decoding table according to RFC-1924
 */
extern utf8_t tvu_base85_dec_table[];

void tvu_base_init(void);

/** Convert 64 bit integer base64.
 * The integer is big endian encoded.
 *
 * @param s     12 byte buffer, includes a nul character.
 * @param _x    64 bit integer to encode.
 * @returns     11 (number of bytes written)
 */
static inline tvu_int tvu_i64_to_base64(utf8_t *s, int64_t _x)
{
    uint64_t x = _x;    // Cast to a unsigned integer.
    int i;

    for (i = 0; i < 11; i++) {
        s[10 - i] = tvu_base64_enc_table[x & 0x3f];
        x >>= 6;
    }
    s[11] = 0;
    return i;
}

/** Convert 64 bit integer to base85.
 * The integer is big endian encoded.
 *
 * @param s     11 byte buffer, includes a nul character.
 * @param _x    64 bit integer to encode.
 * @returns     10 (number of bytes written)
 */
static inline tvu_int tvu_i64_to_base85(utf8_t *s, int64_t _x)
{
    uint64_t x = _x;    // Cast to a unsigned integer.
    int i;

    for (i = 0; i < 10; i++) {
        s[9 - i] = tvu_base85_enc_table[x % 85];
        x /= 85;
    }
    s[10] = 0;
    return i;
}

#endif

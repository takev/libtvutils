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
#ifndef TVU_INTEGER_H
#define TVU_INTEGER_H
#define TVU_INTEGER_H_IN

#include <stdint.h>

/** Sign extend a integer.
 * The integer of 'b' bits is stored in x. A signed integer is returned.
 * @param x     The integer to sign extend.
 * @param b     The number of bits of the integer.
 * @returns     The signed integer.
 */
static inline int64_t tvu_sign_extend(uint64_t x, unsigned int b)
{
    unsigned int m = (sizeof (x) * 8) - b;
    return (int64_t)(x << m) >> m;
}

#endif

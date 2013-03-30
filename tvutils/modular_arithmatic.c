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
#include <tvutils/types.h>

/** The complete 2048 bits number is in little endian.
 * v[0] is the least significant 64 bit word.
 */



uint2048_t tvu_modular_srl1(uint2048_t a)
{
    uint2048_t  result;
    int i;

    for (i = 0; i < lengthof(a.v) - 1; i++) {
        result.v[i] = (a.v[i] >>= 1) | (a.v[i+1] << (bitsof(*(a.v)) - 1));
    }
    result.v[i] = (a.v[i] >>= 1);
    return result;
}

int tvu_modular_and1(uint2048_t a)
{
    return a.v[0] & 1;
}

int tvu_modular_gt0(uint2048_t a)
{
    for (i = 0; i < lengthof(a.v); i++)
        if (a.v[i]) {
            return 1;
        }
    }
    return 0;
}

uint2048_t tvu_modular_mul(uint2048_t a, uint2048_t b, uint2048_t modulus)
{
    uint2048_t result;

    return result;
}

uint2048_t tvu_modular_pow(uint2048_t base, uint2048_t exponent, uint2048_t modulus)
{
    uint2048_t result = 1;

    while (tvu_modular_gt0(exponent)) {
        if (tvu_modular_and1(exponent)) {
            result = tvu_modular_mul(result, base, modulus);
        }
        exponent = tvu_modular_srl1(exponent);
        base = tvu_modular_mul(base, base, modulus);
    }
    return result;
}



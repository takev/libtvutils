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

/*uint2048_t diffie_hellman_group = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xC90FDAA2, 0x2168C234, 0xC4C6628B, 0x80DC1CD1,
    0x29024E08, 0x8A67CC74, 0x020BBEA6, 0x3B139B22, 0x514A0879, 0x8E3404DD,
    0xEF9519B3, 0xCD3A431B, 0x302B0A6D, 0xF25F1437, 0x4FE1356D, 0x6D51C245,
    0xE485B576, 0x625E7EC6, 0xF44C42E9, 0xA637ED6B, 0x0BFF5CB6, 0xF406B7ED,
    0xEE386BFB, 0x5A899FA5, 0xAE9F2411, 0x7C4B1FE6, 0x49286651, 0xECE45B3D,
    0xC2007CB8, 0xA163BF05, 0x98DA4836, 0x1C55D39A, 0x69163FA8, 0xFD24CF5F,
    0x83655D23, 0xDCA3AD96, 0x1C62F356, 0x208552BB, 0x9ED52907, 0x7096966D,
    0x670C354E, 0x4ABC9804, 0xF1746C08, 0xCA18217C, 0x32905E46, 0x2E36CE3B,
    0xE39E772C, 0x180E8603, 0x9B2783A2, 0xEC07A28F, 0xB5C55DF0, 0x6F4C52C9,
    0xDE2BCBF6, 0x95581718, 0x3995497C, 0xEA956AE5, 0x15D22618, 0x98FA0510,
    0x15728E5A, 0x8AACAA68, 0xFFFFFFFF, 0xFFFFFFFF
};*/


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



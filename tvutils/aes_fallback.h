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
#ifndef TVU_AES_X86NI_H
#define TVU_AES_X86NI_H

#ifndef TVU_AES_H_IN
#error "Do not manually include this file, it should be included only from tvutils/aes.h"
#endif

#include <stdlib.h>

static inline int64x2x11_t tvu_aes128_key_schedule(const uint8_t *_key)
{
    abort();
}

static inline int64x2x8_t tvu_aes_white_wash(int64x2x8_t t, int64x2_t key, int nr_blocks)
{
    abort();
}

static inline int64x2x8_t tvu_aes_encrypt_round(int64x2x8_t t, int64x2_t key, int nr_blocks)
{
    abort();
}

static inline int64x2x8_t tvu_aes_encrypt_last_round(int64x2x8_t t, int64x2_t key, int nr_blocks)
{
    abort();
}


#endif

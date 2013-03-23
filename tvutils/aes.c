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
#include <tvutils/aes.h>

int64x2x8_t tvu_aes_encrypt_step(int64x2x8_t src, int64x2x11_t key_schedule, int nr_blocks)
{
    int64x2x8_t tmp;

    tmp = tvu_aes_white_wash(src, key_schedule.v[0], nr_blocks);

    // Do 9 rounds, in reverse.
    for (int round_nr = 1; round_nr < 10; round_nr++) {
        tmp = tvu_aes_encrypt_round(tmp, key_schedule.v[round_nr], nr_blocks);
    }

    // Do the last round in reverse.
    return tvu_aes_encrypt_last_round(tmp, key_schedule.v[10], nr_blocks);
}

int64x2_t tvu_aes_hash_step(int64x2_t hash, int64x2_t src)
{
    int64x2x11_t    key_schedule = tvu_aes_key_schedule(src);
    int64x2x8_t     blocks;

    blocks.v[0] = hash;
    blocks = tvu_aes_encrypt_step(blocks, key_schedule, 1);
    blocks = tvu_aes_white_wash(blocks, hash, 1);
    return blocks.v[0];
}


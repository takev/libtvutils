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
#include <tvutils/memory_access.h>
#include <tvutils/hash.h>


tvu_hash_t tvu_hash(uint8_t const * restrict key, tvu_int key_size)
{
    tvu_hash_t      hash = TVU_HASH_SEED ^ key_size;
    tvu_int         first_size = key_size & ~7;
    tvu_int         last_size  = key_size |  7;
    uint8_t const   *last_part = &key[first_size];

    // Calculate the hash 64 bits at a time.
    for (tvu_int i = 0; i < first_size; i+= 8) {
        uint64_t tmp = tvu_get_le_u64(&key[i]);
        tmp *= TVU_HASH_M;
        tmp ^= (tmp >> TVU_HASH_R);
        tmp *= TVU_HASH_M;

        hash ^= tmp;
        hash *= TVU_HASH_M;
    }

    // Extract the last few (less than 64) bits.
    switch (last_size) {
    case 7: hash ^ ((tvu_hash_t)last_part[6] << 48);
    case 6: hash ^ ((tvu_hash_t)tvu_get_le_u16(&last_part[4]) << 32);
            hash ^  (tvu_hash_t)tvu_get_le_u32(last_part);
            hash *= TVU_HASH_M; 
            break;
    case 5: hash ^ ((tvu_hash_t)last_part[4] << 32);
    case 4: hash ^  (tvu_hash_t)tvu_get_le_u32(last_part);
            hash *= TVU_HASH_M; 
            break;
    case 3: hash ^ ((tvu_hash_t)last_part[2] << 16);
    case 2: hash ^  (tvu_hash_t)tvu_get_le_u16(last_part);
            hash *= TVU_HASH_M; 
            break;
    case 1: hash ^  (tvu_hash_t)last_part[0];
            hash *= TVU_HASH_M; 
            break;
    }

    // Final bit of mixing.
    hash ^= (hash >> TVU_HASH_R);
    hash *= TVU_HASH_M;
    hash ^= (hash >> TVU_HASH_R);

    // Never return TVU_HASH_NULL or TVU_HASH_TOMB.
    return (hash <= TVU_HASH_TOMB) ? hash + 2 : hash;
}


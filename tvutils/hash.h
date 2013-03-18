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
#ifndef TVU_HASH_H
#define TVU_HASH_H

#include <string.h>
#include <tvutils/types.h>

typedef uint64_t tvu_hash_t;                //< Zero is not allowed in the hash.

// The following values must be 0 and 1, or the hash function will not work correctly.
#define TVU_HASH_NULL 0                     //< A not existed hash value, to be used as empty.
#define TVU_HASH_TOMB (TVU_HASH_NULL + 1)   //< A not existed hash value, to be used as tombstone.


#define TVU_HASH_SEED 0                     //< MurmurHash64A seed value. This may be modified in future libraries.
#define TVU_HASH_M    0xc6a4a7935bd1e995    //< MurmurHash64A m value.
#define TVU_HASH_R    47                    //< MurmurHash64A r value.

/** Construct a hash value from a key.
 * This function handles 64 bits at a time and is based on MurmurHash64A.
 * It has a slight modification so it will never return TVU_HASH_NULL or TVU_HASH_TOMB.
 *
 * @param key       The key to hash.
 * @param key_size  The length of the key.
 * @returns         The hash value of the key, will never be TVU_HASH_NULL or TVU_HASH_TOMB.
 */
tvu_hash_t tvu_hash(uint8_t const * restrict key, tvu_int key_size);

/** Construct a hash value from a key.
 * This function handles 64 bits at a time and is based on MurmurHash64A.
 * It has a slight modification so it will never return TVU_HASH_NULL or TVU_HASH_TOMB.
 *
 * @param key       The null terminated key to hash.
 * @returns         The hash value of the key, will never be TVU_HASH_NULL or TVU_HASH_TOMB.
 */
static inline tvu_hash_t tvu_hash_string(utf8_t const * restrict key)
{
    return tvu_hash((uint8_t const *)key, strlen(key));
}

/** Extract an index from the hash value.
 * @param hash          The hash value
 * @param nr_buckets    The number of buckets to get an index for.
 * @returns             An index into a table of buckets.
 */
static inline tvu_int tvu_hash_truncate(tvu_hash_t hash, tvu_int nr_buckets)
{
    return hash % nr_buckets;
}


#endif

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
#ifndef TVU_PERMHASHTABLE_H
#define TVU_PERMHASHTABLE_H

#include <stddef.h>
#include <stdio.h>

/** The permanent hash table.
 * Allocation of the hash table is up to the user.
 * This struct is a incomplete type with a data field which extends to
 * the amount that was allocated. Use tvu_permhashtable_init() to initialize this
 * structure.
 *
 * Unlike many hash tables, the keys and values are both stored inside the hash table.
 * This makes it easy to use in shared-memory. A read on a hash table should be
 * wait-free, CAS is used for going to the next entry, not for looping. But only
 * one writer is allowed at a time.
 *
 * This hash table can only be used for inserts and reads, no data may be deleted.
 * The hash table can't grow or shrink.
 */
typedef struct {
    uint64_t    lock;           //< Index of where the data is free to be used by the producer again.
    uint64_t    size;           //< The number of bytes the array data is.
    uint8_t     key_value_size; //< Size of keys and value appended to each other, stored in the hash table. Must be multiple of 8.
    uint8_t     _padding1;
    uint16_t    _padding2;
    uint32_t    _padding3;
    uint8_t     data[];         //< The data array. Aligned to 64 bits, all entries are aligned to 64 bits.
} tvu_permhashtable_t;


/** A packet in the ring buffer.
 * A packet is always aligned to a 32 bit word boundary.
 * The size_and_flags fields should only be accessed by
 * the utility function operating on this packet.
 * The data is completely user defined.
 */
typedef struct {
    uint8_t     value_size;     //< Value size, 0 means entry is not in use.
    uint8_t     key_size;       //< Key size, 0 means entry is busy.
    uint16_t    _padding2;
    uint32_t    _padding3;
    uint64_t    hash;           //< Hash value for this entry.
    uint8_t     value[];        //< value first, followed by key. Value is first so that it is aligned to 64 bits.
} tvu_permhashtable_entry_t;

/** Return the size of the header of a hash table entry.
 * @returns size of a tvu_permhashtable_entry_t header.
 */
static inline size_t tvu_permhashtable_entry_hdrsize(void)
{
    return offsetof(tvu_permhashtable_entry_t, value);
}

/** Return the size of the header of a ringpacket.
 * @returns size of a tvu_ringpacket_t header.
 */
static inline size_t tvu_permhashtable_entry_size(tvu_permhashtable_t *table)
{
    return tvu_permhashtable_entry_hdrsize() + table->key_value_size;
}

/** Return the a pointer to the key in an entry.
 * @param entry     The hash table entry.
 * @returns         The key in the hash table entry.
 */
static inline uint8_t *tvu_permhashtable_entry_key(tvu_permhashtable_entry_t *entry)
{
    return &entry->value[entry->value_size];
}

/** Write all the data in the hash table entry.
 * The order of writes, and handling of contention is important.
 * When an entry is contented, the function spin-locks until another
 * thread has finished with the entry.
 *
 * @param entry         The entry to update.
 * @param hash          The hash value.
 * @param key           The key for the entry.
 * @param key_size      The size of the key.
 * @param value         The value of the entry.
 * @param value_size    The value size.
 * @returns             True when the entry was written. False if another thread has beat us to it.
 */
bool tvu_permhashtable_entry_write(tvu_permhashtable_entry_t *entry, uint64_t hash, uint8_t const * restrict key, uint8_t key_size, uint8_t const * restrict value, uint8_t value_size);

#endif

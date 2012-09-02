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
#include <tvutils/assert.h>
#include <tvutils/macros.h>
#include <tvutils/atomic.h>
#include <tvutils/hash.h>
#include <tvutils/string.h>
#include <tvutils/permhashtable.h>


tvu_find_t tvu_permhashtable_entry_write(tvu_permhashtable_entry_t * restrict entry, tvu_hash_t hash, uint8_t const * restrict key, tvu_int key_size, uint8_t const * restrict value, tvu_int value_size)
{
    TVU_ASSERT(entry != NULL)
    TVU_ASSERT(key != NULL)
    TVU_ASSERT(value_size == 0 || value != NULL)
    TVU_ASSERT(hash != TVU_HASH_NULL)
    TVU_ASSERT(hash != TVU_HASH_TOMB)
    TVU_ASSERT(key_size > 0)
    TVU_ASSERT(value_size >= 0)

    if (likely(tvu_atomic_cas_u64(&entry->hash, TVU_HASH_NULL, hash))) {
        // We were able to set the hash on this entry, it is now ours.
        tvu_memcpy_null(&entry->value, value, value_size);
        entry->value_size = value_size;
        memcpy(tvu_permhashtable_entry_key(entry), key, key_size);
        // This last write will make the entry done.
        tvu_atomic_write_u32(&entry->key_size, key_size);
        return TVU_FIND_FOUND;

    } else if (likely(tvu_atomic_read_u64(&entry->hash) != hash)) {
        // The other thread was not writing the same value as us, so we failed writing this entry, try another.
        return TVU_FIND_NEXT;

    } else {
        // Someone else beats us on writing the same hash on this entry.
        // We need to check if it has the same key and value, so wait until the key_size is written.
        tvu_permhashtable_entry_wait(entry);

        // Check the key.
        if (entry->key_size != key_size || memcmp(tvu_permhashtable_entry_key(entry), key, key_size) != 0) {
            // Key is different try another entry.
            return TVU_FIND_NEXT;
        }

        // Check the value.
        if (entry->value_size != value_size || memcmp(entry->value, value, value_size) != 0) {
            // The key already exists and the value is different. We can not overwrite old values.
            return TVU_FIND_END;
        }

        // Same key and value, no one needs to know we didn't really write anything.
        return TVU_FIND_FOUND;
    }
}

tvu_find_t tvu_permhashtable_entry_read(tvu_permhashtable_entry_t * restrict entry, tvu_hash_t hash, uint8_t const * restrict key, tvu_int key_size)
{
    TVU_ASSERT(entry != NULL)
    TVU_ASSERT(key != NULL)
    TVU_ASSERT(hash != TVU_HASH_NULL)
    TVU_ASSERT(hash != TVU_HASH_TOMB)
    TVU_ASSERT(key_size > 0)

    switch (tvu_atomic_read_u64(&entry->hash)) {
    case TVU_HASH_NULL:
        return TVU_FIND_END;  // Empty entry, this means we can't find the hash.
    case TVU_HASH_TOMB:
        return TVU_FIND_NEXT; // Tomb stone, we should look further.
    default:
        // By comparing the key size with our own, we also check if the entry is completed by another thread.
        if ((tvu_atomic_read_u32(&entry->key_size) == key_size) && (memcmp(tvu_permhashtable_entry_key(entry), key, key_size) == 0)) {
            return TVU_FIND_FOUND;
        } else {
            return TVU_FIND_NEXT;
        }
    }
}


tvu_permhashtable_entry_t *tvu_parmhashtable_set(tvu_permhashtable_t * restrict table, tvu_hash_t hash, uint8_t const * restrict key, uint8_t key_size, uint8_t const * restrict value, uint8_t value_size)
{
    TVU_ASSERT(table != NULL)
    TVU_ASSERT(key != NULL)
    TVU_ASSERT(hash != TVU_HASH_NULL)
    TVU_ASSERT(hash != TVU_HASH_TOMB)
    TVU_ASSERT(key_size > 0)

    tvu_int                     i      = tvu_hash_truncate(hash, table->nr_entries);
    tvu_permhashtable_entry_t   *entry = tvu_permhashtable_get_entry(table, i);;

retry:
    // Just try to write in the first entry, write will fail if there is already something there.
    switch (tvu_permhashtable_entry_write(entry, hash, key, key_size, value, value_size)) {
    case TVU_FIND_NEXT:
        // Try the next entries.
        i = TVU_INCWRAP(i, table->nr_entries);
        entry = tvu_permhashtable_get_entry(table, i);
        goto retry;
    case TVU_FIND_FOUND:
        // Entry found, return it.
        return entry;
    case TVU_FIND_END:
        // Key was found, but value was set different.
        return NULL;
    }
}

tvu_permhashtable_entry_t *tvu_parmhashtable_get(tvu_permhashtable_t const * restrict table, tvu_hash_t hash, uint8_t const * restrict key, uint8_t key_size)
{
    TVU_ASSERT(table != NULL)
    TVU_ASSERT(key != NULL)
    TVU_ASSERT(hash != TVU_HASH_NULL)
    TVU_ASSERT(hash != TVU_HASH_TOMB)
    TVU_ASSERT(key_size > 0)

    tvu_int                     i      = tvu_hash_truncate(hash, table->nr_entries);
    tvu_permhashtable_entry_t   *entry = tvu_permhashtable_get_entry(table, i);;

retry:
    switch (tvu_permhashtable_entry_read(entry, hash, key, key_size)) {
    case TVU_FIND_NEXT:
        // Try the next entries.
        i = TVU_INCWRAP(i, table->nr_entries);
        entry = tvu_permhashtable_get_entry(table, i);
        goto retry;
    case TVU_FIND_FOUND:
        // Entry found, return it.
        return entry;
    case TVU_FIND_END:
        // Entry not found.
        return NULL;
    }
}


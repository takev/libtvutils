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
#include <tvutils/atomic.h>
#include <tvutils/hash.h>
#include <tvutils/permhashtable.h>


bool tvu_permhashtable_entry_write(tvu_permhashtable_entry_t *entry, uint64_t hash, uint8_t const * restrict key, uint8_t key_size, uint8_t const * restrict value, uint8_t value_size)
{
    if (tvu_atomic_cas_u64(&entry->hash, TVU_HASH_NULL, hash)) {
        // We were able to set the hash on this entry, it is now ours.
        memcpy(&entry->value, value, value_size);
        entry->value_size = value_size;
        memcpy(tvu_permhashtable_entry_key(entry), key, key_size);
        // This last write will make the entry done.
        tvu_atomic_write_u8(&entry->key_size, key_size);
        return true;

    } else if (tvu_atomic_read_u64(&entry->hash) != hash) {
        // The other thread was not writing the same value as us, so we failed writing this entry.
        return false;

    } else {
        // Someone else beats us on writing the same hash on this entry.
        // We need to check if it has the same key, so wait until the key is written.
        while (tvu_atomic_read_u8(&entry->key_size) == 0) {
            tvu_atomic_pause();
        }
        if (memcmp(tvu_permhashtable_entry_key(entry), key, key_size) == 0) {
            return false;
        }

        // It seems that another thread created the exact same entry, lie and say we succeeded,
        // no one needs to know.
        return true;
    }
}


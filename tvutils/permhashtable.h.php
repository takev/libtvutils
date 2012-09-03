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
#include <errno.h>
#include <tvutils/hash.h>
#include <tvutils/number.h>

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
    uint32_t    entry_size;     //< Size of each entry, including header.
    uint32_t    nr_entries;     //< Nr of entries in the table.
    uint8_t     data[];         //< The data array. Aligned to 64 bits, all entries are aligned to 64 bits.
} tvu_permhashtable_t;


/** A packet in the ring buffer.
 * A packet is always aligned to a 32 bit word boundary.
 * The size_and_flags fields should only be accessed by
 * the utility function operating on this packet.
 * The data is completely user defined.
 */
typedef struct {
    uint32_t    value_size;     //< Value size, 0 means entry is not in use.
    uint32_t    key_size;       //< Key size, 0 means entry is busy.
    tvu_hash_t  hash;           //< Hash value for this entry.
    uint8_t     value[];        //< value first, followed by key. Value is first so that it is aligned to 64 bits.
} tvu_permhashtable_entry_t;

/** Return the size of the header of a hash table entry.
 * @returns size of a tvu_permhashtable_entry_t header.
 */
static inline size_t tvu_permhashtable_entry_hdrsize(void)
{
    return offsetof(tvu_permhashtable_entry_t, value);
}

/** Return the size of the header of a hash table.
 * @returns size of a tvu_permhashtable_t header.
 */
static inline size_t tvu_permhashtable_hdrsize(void)
{
    return offsetof(tvu_permhashtable_t, data);
}

/** Calculate the size of an entry.
 * @param max_key_size      Maximum size of a key.
 * @param max_value_size    Maximum size of a value.
 * @returns                 Size of an entry, rounded-up to 64 bits.
 */
static inline size_t tvu_permhashtable_entry_size(size_t max_key_size, size_t max_value_size)
{
    return tvu_round_up_u64(max_key_size + max_value_size + tvu_permhashtable_entry_hdrsize(), sizeof (uint64_t));
}

/** Check if the hash table is initialized.
 * @param self              Pointer to a (un)initialized hash table.
 * @param size              Memory size of the hash table.
 * @param max_key_size      Maximum size of a key.
 * @param max_value_size    Maximum size of a value.
 * @returns     true if initialized, false if not.
 */
static inline bool tvu_permhashtable_is_initialized(tvu_permhashtable_t *table, size_t size, size_t max_key_size, size_t max_value_size)
{
    uint32_t entry_size = tvu_permhashtable_entry_size(max_key_size, max_value_size);
    if (table->entry_size != entry_size) {
        return false;
    }

    uint32_t nr_entries = (size - tvu_permhashtable_hdrsize()) / entry_size;
    if (table->nr_entries != nr_entries) {
        return false;
    }
    return true;
}

/** Initialize the ring buffer.
 * @param self              Pointer to a uninitialized and zeroed out hash table.
 * @param size              Memory size of the hash table.
 * @param max_key_size      Maximum size of a key.
 * @param max_value_size    Maximum size of a value.
 * @return                  0 if ok, -1 on error.
 */
static inline tvu_int tvu_permhashtable_init(tvu_permhashtable_t *table, size_t size, size_t max_key_size, size_t max_value_size)
{
    uint32_t entry_size = tvu_permhashtable_entry_size(max_key_size, max_value_size);
    uint32_t nr_entries = (size - tvu_permhashtable_hdrsize()) / entry_size;

    if (nr_entries < 1) {
        errno = EINVAL;
        return -1;
    }

    table->entry_size = entry_size;
    table->nr_entries = nr_entries;
    return 0;
}

/** Return the a pointer to the key in an entry.
 * @param entry     The hash table entry.
 * @returns         The key in the hash table entry.
 */
static inline uint8_t *tvu_permhashtable_entry_key(tvu_permhashtable_entry_t *entry)
{
    return &entry->value[entry->value_size];
}

/** Wait until this hash table entry is written by another thread.
 * @param entry     The hash table entry.
 */
static inline void tvu_permhashtable_entry_wait(tvu_permhashtable_entry_t *entry)
{
    while (tvu_atomic_read_u32(&entry->key_size) == 0) {
        tvu_atomic_pause();
    }
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
 * @returns             TVU_FIND_NEXT if the entry does not match the key, TVU_FIND_FOUND if the entry matched key and value, TVU_FIND_END if the entries value does not match.
 */
tvu_find_t tvu_permhashtable_entry_write(tvu_permhashtable_entry_t * restrict entry, tvu_hash_t hash, uint8_t const * restrict key, tvu_int key_size, uint8_t const * restrict value, tvu_int value_size);


/** Check the data in the hash table entry.
 * The order of reads, and handling of contention is important.
 * When an entry is contented, the function skips the entry, to look for the next, this function is wait-free.
 *
 * @param entry         The entry to update.
 * @param hash          The hash value.
 * @param key           The key for the entry.
 * @param key_size      The size of the key.
 * @returns             TVU_FIND_NEXT if the entry does not match the key, TVU_FIND_FOUND if the entry does match the key, TVU_FIND_END if the entry is empty.
 */
tvu_find_t tvu_permhashtable_entry_read(tvu_permhashtable_entry_t * restrict entry, tvu_hash_t hash, uint8_t const * restrict key, tvu_int key_size);


/** Delete a hash table entry.
 * The hash table entry is stomped over with a tombstone.
 * Any reading process should first read the hash entry; all other entries should remain valid for read during contention.
 * This entry is deleted entry is never reclaimed.
 */
void tvu_permhashtable_entry_delete(tvu_permhashtable_entry_t * restrict entry)
{
    tvu_atomic_write_u64(&entry->hash, TVU_HASH_TOMB);
}

/** Get a hash table entry.
 * @param table     Hash table.
 * @param i         Index in the hash table.
 * @returns         Pointer to a hash table entry.
 */
static inline tvu_permhashtable_entry_t *tvu_permhashtable_get_entry(tvu_permhashtable_t const * restrict table, tvu_int i)
{
    return (tvu_permhashtable_entry_t *)&table->data[i * table->entry_size];
}

/** Set an entry in the hash table.
 *
 * @param table         Hash table.
 * @param hash          Hash value.
 * @param key           The key the entry should be saved under.
 * @param key_size      Size of the key.
 * @param value         The value that should be saved.
 * @param value_size    The size of the value.
 * @returns             The entry where the value was saved, NULL if the key was already set with a different value.
 */
tvu_permhashtable_entry_t *tvu_permhashtable_set(tvu_permhashtable_t * restrict table, tvu_hash_t hash, uint8_t const * restrict key, uint8_t key_size, uint8_t const * restrict value, uint8_t value_size);

tvu_permhashtable_entry_t *tvu_permhashtable_get(tvu_permhashtable_t const * restrict table, tvu_hash_t hash, uint8_t const * restrict key, uint8_t key_size);


/** Set an entry in the hash table.
 *
 * @param table         Hash table.
 * @param key           The key the entry should be saved under.
 * @param value         The value that should be saved.
 * @returns             The entry where the value was saved, NULL if the key was already set with a different value.
 */
static inline utf8_t const *tvu_permhashtable_set_s_s(tvu_permhashtable_t * restrict table, utf8_t const * restrict key, utf8_t const * restrict value)
{
    tvu_int                     key_size   = strlen(key);
    tvu_int                     value_size = strlen(value) + 1; // Add null termination to the value, so it can be easily returned.
    tvu_hash_t                  hash       = tvu_hash((uint8_t const *)key, key_size);
    tvu_permhashtable_entry_t   *entry     = tvu_permhashtable_set(table, hash, (uint8_t const *)key, key_size, (uint8_t const *)value, value_size);

    return (utf8_t const *)(entry ? entry->value : NULL);
}


static inline utf8_t const *tvu_permhashtable_get_s_s(tvu_permhashtable_t const * restrict table, utf8_t const * restrict key)
{
    tvu_int                     key_size = strlen(key);
    tvu_hash_t                  hash     = tvu_hash((uint8_t const *)key, key_size);
    tvu_permhashtable_entry_t   *entry   = tvu_permhashtable_get(table, hash, (uint8_t const *)key, key_size);

    return (utf8_t const *)(entry ? entry->value : NULL);
}

<?php
$bit_sizes = array(8, 16, 32, 64);
$sizes = array(2, 4);
$signs = array("i", "u");
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
    foreach ($signs as $sign) {
        $_sign = $sign == "u" ? "u" : "";
        $__sign = $sign == "u" ? "unsigned" : "signed";
        $short_type = $sign . $bit_size;
        $c_type = $_sign . "int" . $bit_size . "_t";
?>
/** Set an entry in the hash table.
 *
 * @param table         Hash table.
 * @param key           The key the entry should be saved under.
 * @param value         The value that should be saved.
 * @returns             The entry where the value was saved, NULL if the key was already set with a different value.
 */
static inline <?=$c_type?> tvu_permhashtable_set_s_<?=$short_type?>(tvu_permhashtable_t * restrict table, utf8_t const * restrict key, <?=$c_type?> value)
{
    tvu_int                     key_size   = strlen(key);
    tvu_hash_t                  hash       = tvu_hash((uint8_t const *)key, key_size);
    tvu_permhashtable_entry_t   *entry     = tvu_permhashtable_set(table, hash, (uint8_t const *)key, key_size, (uint8_t const *)&value, sizeof (<?=$c_type?>));

    return entry ? *(<?=$c_type?> *)entry->value : 0;
}


static inline <?=$c_type?> tvu_permhashtable_get_s_<?=$short_type?>(tvu_permhashtable_t const * restrict table, utf8_t const * restrict key)
{
    tvu_int                     key_size = strlen(key);
    tvu_hash_t                  hash     = tvu_hash((uint8_t const *)key, key_size);
    tvu_permhashtable_entry_t   *entry   = tvu_permhashtable_get(table, hash, (uint8_t const *)key, key_size);

    return entry ? *(<?=$c_type?> *)entry->value : 0;
}
<?php }} ?>

#endif

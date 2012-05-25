/* libuniqueid - Library for distributed generating unique idententifiers
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
#ifndef UNIQUEID_ATOMIC_H
#define UNIQUEID_ATOMIC_H

#include <stdint.h>
#include <stdbool.h>

/** Atomic 64 bit read.
 * After the read a full memory barrier is ordered.
 *
 * @param ptr   Pointer to a 64 bit integer.
 * @returns     The 64 bit integer read from ptr.
 */
static inline int64_t uniq_atomic_read_i64(volatile int64_t *ptr)
{
    int64_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a 64 bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 64 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline bool uniq_atomic_add_read_i64(volatile int64_t *ptr, int64_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a 64 bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 64 bit integer.
 * @param oldvalue  Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool uniq_atomic_cas_i64(volatile int64_t *ptr, int64_t oldval, int64_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

#endif

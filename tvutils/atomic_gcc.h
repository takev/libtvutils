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
#ifndef TVU_ATOMIC_GCC_H
#define TVU_ATOMIC_GCC_H

#ifndef TVU_ATOMIC_H_IN
#error "Do not manually include this file, it should be included only from tvutils/atomic.h"
#endif

#include <stdint.h>
#include <stdbool.h>

/** Atomic signed 8-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 8 bit integer.
 * @returns     The 8 bit integer read from ptr.
 */
static inline int8_t tvu_atomic_read_i8(volatile int8_t *ptr)
{
    int8_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a signed 8-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 8 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline int8_t tvu_atomic_add_read_i8(volatile int8_t *ptr, int8_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a signed 8-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 8 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_i8(volatile int8_t *ptr, int8_t oldval, int8_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

/** Atomic unsigned 8-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 8 bit integer.
 * @returns     The 8 bit integer read from ptr.
 */
static inline uint8_t tvu_atomic_read_u8(volatile uint8_t *ptr)
{
    uint8_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a unsigned 8-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 8 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline uint8_t tvu_atomic_add_read_u8(volatile uint8_t *ptr, uint8_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a unsigned 8-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 8 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_u8(volatile uint8_t *ptr, uint8_t oldval, uint8_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

/** Atomic signed 16-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 16 bit integer.
 * @returns     The 16 bit integer read from ptr.
 */
static inline int16_t tvu_atomic_read_i16(volatile int16_t *ptr)
{
    int16_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a signed 16-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 16 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline int16_t tvu_atomic_add_read_i16(volatile int16_t *ptr, int16_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a signed 16-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 16 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_i16(volatile int16_t *ptr, int16_t oldval, int16_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

/** Atomic unsigned 16-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 16 bit integer.
 * @returns     The 16 bit integer read from ptr.
 */
static inline uint16_t tvu_atomic_read_u16(volatile uint16_t *ptr)
{
    uint16_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a unsigned 16-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 16 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline uint16_t tvu_atomic_add_read_u16(volatile uint16_t *ptr, uint16_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a unsigned 16-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 16 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_u16(volatile uint16_t *ptr, uint16_t oldval, uint16_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

/** Atomic signed 32-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 32 bit integer.
 * @returns     The 32 bit integer read from ptr.
 */
static inline int32_t tvu_atomic_read_i32(volatile int32_t *ptr)
{
    int32_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a signed 32-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 32 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline int32_t tvu_atomic_add_read_i32(volatile int32_t *ptr, int32_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a signed 32-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 32 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_i32(volatile int32_t *ptr, int32_t oldval, int32_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

/** Atomic unsigned 32-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 32 bit integer.
 * @returns     The 32 bit integer read from ptr.
 */
static inline uint32_t tvu_atomic_read_u32(volatile uint32_t *ptr)
{
    uint32_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a unsigned 32-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 32 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline uint32_t tvu_atomic_add_read_u32(volatile uint32_t *ptr, uint32_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a unsigned 32-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 32 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_u32(volatile uint32_t *ptr, uint32_t oldval, uint32_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

/** Atomic signed 64-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 64 bit integer.
 * @returns     The 64 bit integer read from ptr.
 */
static inline int64_t tvu_atomic_read_i64(volatile int64_t *ptr)
{
    int64_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a signed 64-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 64 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline int64_t tvu_atomic_add_read_i64(volatile int64_t *ptr, int64_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a signed 64-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 64 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_i64(volatile int64_t *ptr, int64_t oldval, int64_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

/** Atomic unsigned 64-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a 64 bit integer.
 * @returns     The 64 bit integer read from ptr.
 */
static inline uint64_t tvu_atomic_read_u64(volatile uint64_t *ptr)
{
    uint64_t x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a unsigned 64-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a 64 bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline uint64_t tvu_atomic_add_read_u64(volatile uint64_t *ptr, uint64_t value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a unsigned 64-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a 64 bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_u64(volatile uint64_t *ptr, uint64_t oldval, uint64_t newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}


#endif

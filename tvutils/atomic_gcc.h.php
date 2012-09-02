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

/** All reads before this barrier can not cross after this barrier.
 */
static inline void tvu_atomic_read_barrier(void)
{
    __sync_synchronize();
}

/** All writes before this barrier can not cross after this barrier.
 */
static inline void tvu_atomic_write_barrier(void)
{
    __sync_synchronize();
}

/** Tell the CPU to wait for a really short time.
 * It tells the CPU that we are spin locking; it can schedule other
 * hyper threads and memory access. Like yield, but faster.
 */
static inline void tvu_atomic_pause(void)
{
    //__builtin_ia32_pause();
}

<?php
$bit_sizes = array(8, 16, 32, 64);
$signs = array("i", "u");
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
    foreach ($signs as $sign) {
        $_sign = $sign == "u" ? "u" : "";
        $__sign = $sign == "u" ? "unsigned" : "signed";
        $tvutype = $sign . $bit_size;
        $ctype = $_sign . "int" . $bit_size . "_t";
?>
/** Atomic <?=$__sign?> <?=$bit_size?>-bit integer read.
 * After the read a full memory barrier is executed.
 *
 * @param ptr   Pointer to a <?=$bit_size?> bit integer.
 * @returns     The <?=$bit_size?> bit integer read from ptr.
 */
static inline <?=$ctype?> tvu_atomic_read_<?=$tvutype?>(volatile <?=$ctype?> *ptr)
{
    <?=$ctype?> x = *ptr;
    __sync_synchronize();
    return x;
}

/** Atomic add then read a <?=$__sign?> <?=$bit_size?>-bit integer.
 * First the add is done into memory, then the result is returned. With
 * a full memory barrier.
 *
 * @param ptr    Pointer to a <?=$bit_size?> bit integer.
 * @param value  The number to add to the ptr.
 * @returns      The result of the addition.
 */
static inline <?=$ctype?> tvu_atomic_add_read_<?=$tvutype?>(volatile <?=$ctype?> *ptr, <?=$ctype?> value)
{
    return __sync_add_and_fetch(ptr, value);
}

/** Atromic compare and swap of a <?=$__sign?> <?=$bit_size?>-bit integer.
 * First compare the memory location with the old value, if they are equal then
 * replace the value in memory with the new value and return true. If they
 * don't compare equal then return false, and without updating the value in memory.
 *
 * @param ptr       Pointer to a <?=$bit_size?> bit integer.
 * @param oldval    Compare the memory with this value.
 * @param newval    Replace value in memory with the new value.
 * @returns         True if memory is updated with newval.
 */
static inline bool tvu_atomic_cas_<?=$tvutype?>(volatile <?=$ctype?> *ptr, <?=$ctype?> oldval, <?=$ctype?> newval)
{
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

<?php }} ?>

#endif

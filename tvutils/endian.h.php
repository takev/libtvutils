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
#ifndef TVU_ENDIAN_H
#define TVU_ENDIAN_H

#if __GLIBC__ >= 2
#include <byteswap.h>
#define tvu_swap_i8(x)  (x)
#define tvu_swap_i16(x) bswap_16(x)
#define tvu_swap_i32(x) bswap_32(x)
#define tvu_swap_i64(x) bswap_64(x)
#define tvu_swap_u8(x)  (x)
#define tvu_swap_u16(x) bswap_16(x)
#define tvu_swap_u32(x) bswap_32(x)
#define tvu_swap_u64(x) bswap_64(x)

#elif defined(__APPLE__)
#include <stdint.h>
#include <machine/endian.h>
#include <machine/byte_order.h>
#define tvu_swap_i8(x)  (x)
#define tvu_swap_i16(x) OSSwapInt16(x)
#define tvu_swap_i32(x) OSSwapInt32(x)
#define tvu_swap_i64(x) OSSwapInt64(x)
#define tvu_swap_u8(x)  (x)
#define tvu_swap_u16(x) OSSwapInt16(x)
#define tvu_swap_u32(x) OSSwapInt32(x)
#define tvu_swap_u64(x) OSSwapInt64(x)

#else
#error "Could not find an endian implementation for this platform."
#endif

<?php
$bit_sizes = array(8, 16, 32, 64);
$endians = array("le", "be");
$signs = array("i", "u");
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
    foreach ($endians as $endian) {
        $__endian = $endian == "be" ? "big endian" : "little endian";
        foreach ($signs as $sign) {
            $_sign = $sign == "u" ? "u" : "";
            $__sign = $sign == "u" ? "unsigned" : "signed";
?>
/** Convert a <?=$__endian?> <?=$bit_size?>-bit <?=$__sign?> integer to host native.
 * @param x     <?=$__endian?> integer to convert to native.
 * @return      Native integer.
 */
static inline <?=$_sign?>int<?=$bit_size?>_t tvu_<?=$endian?>toh_<?=$sign?><?=$bit_size?>(<?=$_sign?>int<?=$bit_size?>_t x)
{
#ifdef TVU_BIG_ENDIAN
    return <?= $endian == "be" ? "x" : ("tvu_swap_" . $sign . $bit_size . "(x)") ?>;
#else
    return <?= $endian == "be" ? ("tvu_swap_" . $sign . $bit_size . "(x)") : "x" ?>;
#endif
}

/** Convert a native <?=$bit_size?>-bit <?=$__sign?> integer to <?=$__endian?>.
 * @param x     Host native integer.
 * @return      <?=$__endian?> integer.
 */
static inline <?=$_sign?>int<?=$bit_size?>_t tvu_hto<?=$endian?>_<?=$sign?><?=$bit_size?>(<?=$_sign?>int<?=$bit_size?>_t x)
{
#ifdef TVU_BIG_ENDIAN
    return <?= $endian == "be" ? "x" : ("tvu_swap_" . $sign . $bit_size) ?>;
#else
    return <?= $endian == "be" ? ("tvu_swap_" . $sign . $bit_size . "(x)") : "x" ?>;
#endif
}

<?php }}} ?>

#endif

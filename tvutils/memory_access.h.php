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
#ifndef TVU_MEMORY_ACCESS_H
#define TVU_MEMORY_ACCESS_H

#include <stdint.h>
#include <tvutils/target.h>
#include <tvutils/stdfloat.h>
#include <tvutils/endian.h>

typedef union {
    char        c[1];
    uint8_t     u;
    int8_t      i;
} union8_t;

typedef union {
    char        c[2];
    uint16_t    u;
    int16_t     i;
} union16_t;

typedef union {
    char        c[4];
    uint32_t    u;
    int32_t     i;
    float       f;
} union32_t;

typedef union {
    char        c[8];
    uint64_t    u;
    int64_t     i;
    double      f;
} union64_t;

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
/** Get an <?=$__endian?> <?=$__sign?> integer from a byte buffer.
 * This function does the correct thing in respect to strict aliasing and unaligned
 * memory access on computer which can not handle this.
 *
 * @param buffer    The memory location where the <?=$__endian?> <?=$__sign?> integer is located.
 * @returns         An host native <?=$bit_size?>-bit <?=$__sign?> integer.
 */
static inline <?=$_sign?>int<?=$bit_size?>_t tvu_get_<?=$endian?>_<?=$sign?><?=$bit_size?>(uint8_t * restrict buffer)
{
#ifdef TVU_ALIGNED_ACCESS
    union<?=$bit_size?>_t conv;
    <?php for ($byte_nr = 0; $byte_nr < $nr_bytes; $byte_nr++) { ?>
    conv.c[<?=$byte_nr?>] = buffer[<?=$byte_nr?>];
    <?php } ?>
    return tvu_<?=$endian?>toh_<?=$sign?><?=$bit_size?>(conv.<?=$sign?>);
#else
    union<?=$bit_size?>_t *conv = (union<?=$bit_size?>_t *)buffer;
    return tvu_<?=$endian?>toh_<?=$sign?><?=$bit_size?>(conv-><?=$sign?>);
#endif
}

/** Set an <?=$__endian?> <?=$__sign?> integer into byte buffer.
 * This function does the correct thing in respect to strict aliasing and unaligned
 * memory access on computer which can not handle this.
 *
 * @param buffer    The memory location where the <?=$__endian?> <?=$__sign?> integer will be written to.
 * @param x         An host native <?=$bit_size?>-bit <?=$__sign?> integer.
 */
static inline void tvu_set_<?=$endian?>_<?=$sign?><?=$bit_size?>(uint8_t * restrict buffer, <?=$_sign?>int<?=$bit_size?>_t x)
{
#ifdef TVU_ALIGNED_ACCESS
    union<?=$bit_size?>_t conv = {.<?=$sign?> = tvu_hto<?=$endian?>_<?=$sign?><?=$bit_size?>(x)};
    <?php for ($byte_nr = 0; $byte_nr < $nr_bytes; $byte_nr++) { ?>
    buffer[<?=$byte_nr?>] = conv.c[<?=$byte_nr?>];
    <?php } ?>
#else
    union<?=$bit_size?>_t *conv = (union<?=$bit_size?>_t *)buffer;
    conv-><?=$sign?> = tvu_hto<?=$endian?>_<?=$sign?><?=$bit_size?>(x);
#endif
}

<?php }}} ?>

<?php
$bit_sizes = array(32, 64);
$endians = array("le", "be");
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
    foreach ($endians as $endian) {
        $__endian = $endian == "be" ? "big endian" : "little endian";
?>
/** Get an <?=$__endian?> float from a byte buffer.
 * This function does the correct thing in respect to strict aliasing and unaligned
 * memory access on computer which can not handle this.
 *
 * @param buffer    The memory location where the <?=$__endian?> float is located.
 * @returns         An host native <?=$bit_size?>-bit float.
 */
static inline float<?=$bit_size?>_t tvu_get_<?=$endian?>_f<?=$bit_size?>(uint8_t * restrict buffer)
{
    union<?=$bit_size?>_t conv;

#ifdef TVU_ALIGNED_ACCESS
<?php for ($i = 0; $i < $nr_bytes; $i++) { ?>
    conv.c[<?=$i?>] = buffer[<?=$i?>];
<?php } ?>
    conv.u = tvu_hto<?=$endian?>_u<?=$bit_size?>(conv.u);
#else
    union<?=$bit_size?>_t *conv2 = (union<?=$bit_size?>_t *)buffer;
    conv.u = tvu_hto<?=$endian?>_u<?=$bit_size?>(conv2->u);
#endif

    return conv.f;
}

/** Set an <?=$__endian?> float into a byte buffer.
 * This function does the correct thing in respect to strict aliasing and unaligned
 * memory access on computer which can not handle this.
 *
 * @param buffer    The memory location where the <?=$__endian?> float should be stored;
 * @param x         An host native <?=$bit_size?>-bit float.
 */
static inline void tvu_set_<?=$endian?>_f<?=$bit_size?>(uint8_t * restrict buffer, float<?=$bit_size?>_t x)
{
    union<?=$bit_size?>_t conv = {.f = x};

#ifdef TVU_ALIGNED_ACCESS
    conv.u = tvu_hto<?=$endian?>_u<?=$bit_size?>(conv.u);
<?php for ($i = 0; $i < $nr_bytes; $i++) { ?>
    buffer[<?=$i?>] = conv.c[<?=$i?>];
<?php } ?>
#else
    union<?=$bit_size?>_t *conv2 = (union<?=$bit_size?>_t *)buffer;
    conv2->u = tvu_hto<?=$endian?>_u<?=$bit_size?>(conv.u);
#endif
}

<?php }} ?>

#endif

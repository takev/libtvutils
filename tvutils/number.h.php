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
#ifndef TVU_NUMBER_H
#define TVU_NUMBER_H

#include <tvutils/macros.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

<?php
$bit_sizes = array(8, 16, 32, 64);
$sizes = array(2, 4);
$signs = array("i", "u");
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
    foreach ($signs as $sign) {
        $_sign = $sign == "u" ? "u" : "";
        $__sign = $sign == "u" ? "unsigned" : "signed";
        foreach ($sizes as $size) {
?>
typedef struct {
     <?=$_sign?>int<?=$bit_size?>_t v[<?=$size?>];
} vec<?=$size?>_<?=$sign?><?=$bit_size?>_t;

<?php }}} ?>

static inline vec2_i32_t tvu_div_i32(int32_t numer, int32_t denom)
{
    div_t       a = div(numer, denom);
    vec2_i32_t  b = {.v = {a.quot, a.rem}};
    return b;
}

static inline vec2_i64_t tvu_div_i64(int64_t numer, int64_t denom)
{
    lldiv_t     a = lldiv(numer, denom);
    vec2_i64_t  b = {.v = {a.quot, a.rem}};
    return b;
}

<?php
$bit_sizes = array(8, 16, 32, 64);
$signs = array("i", "u");
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
    foreach ($signs as $sign) {
        $_sign = $sign == "u" ? "u" : "";
        $__sign = $sign == "u" ? "unsigned" : "signed";
?>

<?php if ($sign == "u") { ?>
/** Sign extend a integer.
 * The integer of 'b' bits is stored in x. A signed integer is returned.
 * @param x     The integer to sign extend.
 * @param b     The number of bits of the integer.
 * @returns     The signed integer.
 */
static inline int<?=$bit_size?>_t tvu_sign_extend_u<?=$bit_size?>(uint<?=$bit_size?>_t x, unsigned int b)
{
    unsigned int m = (sizeof (x) * 8) - b;
    return (int<?=$bit_size?>_t)(x << m) >> m;
}

/** Round up integer.
 * 
 * @param x     The integer to round up.
 * @param m     Round to this modulo.
 */
static inline uint<?=$bit_size?>_t tvu_round_up_u<?=$bit_size?>(uint<?=$bit_size?>_t x, unsigned int m)
{
    return ((x + (m - 1)) / m) * m;
}

/** Round down integer.
 * 
 * @param x     The integer to round down.
 * @param m     Round to this modulo.
 */
static inline uint<?=$bit_size?>_t tvu_round_down_u<?=$bit_size?>(uint<?=$bit_size?>_t x, unsigned int m)
{
    return (x / m) * m;
}
<?php } ?>

/** Get the minimum of two integers.
 * @param a     The integer.
 * @param b     The integer.
 * @returns     the smallest integer.
 */
static inline uint<?=$bit_size?>_t tvu_min_<?=$sign?><?=$bit_size?>(<?=$_sign?>int<?=$bit_size?>_t a, <?=$_sign?>int<?=$bit_size?>_t b)
{
    return TVU_MIN(a, b);
}

/** Get the maximum of two integers.
 * @param a     The integer.
 * @param b     The integer.
 * @returns     the largest integer.
 */
static inline uint<?=$bit_size?>_t tvu_max_<?=$sign?><?=$bit_size?>(<?=$_sign?>int<?=$bit_size?>_t a, <?=$_sign?>int<?=$bit_size?>_t b)
{
    return TVU_MAX(a, b);
}

/** Get the absolute value of an integer.
 * @param x     The signed integer.
 * @returns     An unsigned integer.
 */
static inline uint<?=$bit_size?>_t tvu_abs_<?=$sign?><?=$bit_size?>(<?=$_sign?>int<?=$bit_size?>_t x)
{
    <?php if ($sign == "i") { ?>
        return TVU_ABS(x);
    <?php } else { ?>
        return x;
    <?php } ?>
}

<?php }} ?>
#endif

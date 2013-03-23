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
#ifndef TVU_BIGINT_H
#define TVU_BIGINT_H

#include <tvutils/macros.h>
#include <tvutils/types.h>
#include <tvutils/compiler.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef uint64_t    bigint_digit_t;
typedef uint128_t   bigint_ddigit_t;

<?php
$bit_sizes = array(64, 128, 256, 512, 1024, 2048, 4096);
foreach ($bit_sizes as $bit_size) {
    $nr_words = $bit_size / 8;
?>

/** An <?=$bit_size?> integer.
 * Internally a carry is kept, which is the size of a digit.
 */
typedef struct {
    bigint_digit_t v[<?=$nr_words?> + 1];
} bigint<?=$bit_size?>_t;

<?php } ?>


<?php
$bit_sizes = array(64, 128, 256, 512, 1024, 2048);
foreach ($bit_sizes as $bit_size) {
    $nr_words = $bit_size / 8;
?>

/** Add two number together.
 */
static inline bigint<?=$bit_size?>_t tvu_bigint_add<?=$bit_size?>(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    int                     i;
    bigint_ddigit_t         tmp;
    bigint<?=$bit_size?>_t  result;
    bigint_digit_t          carry;

    carry = 0;
    for (i = 0; i < lengthof(a.v); i++) {
        tmp = (bigint_ddigit_t)a.v[i] + b.v[i] + carry;
        result.v[0] = (bigint_digit_t)tmp;
        carry = tmp >> bitsof(bigint_digit_t);
    }
    return result;
}

/** Accumelate numbers.
 */
static inline void tvu_bigint_acc<?=$bit_size?>(bigint<?=$bit_size?>_t *a, bigint<?=$bit_size?>_t b)
{
    int                   i;
    bigint_ddigit_t       tmp;
    bigint_digit_t        carry;

    carry = 0;
    for (i = 0; i < lengthof(a->v); i++) {
        tmp = (bigint_ddigit_t)a->v[i] + b.v[i] + carry;
        a->v[i] = (bigint_digit_t)tmp;
        carry = tmp >> bitsof(bigint_digit_t);
    }
}

static inline void tvu_bigint_fma<?=$bit_size?>_digit(bigint<?=$bit_size*2?>_t *r, bigint<?=$bit_size?>_t a, bigint_digit_t b, int digit_nr)
{
    int                       i;
    bigint_ddigit_t           mul_tmp;
    bigint_ddigit_t           acc_tmp;
    bigint_digit_t            mul_carry;
    bigint_digit_t            acc_carry;

    mul_carry = 0;
    acc_carry = 0;
    for (i = 0; i < lengthof(a.v); i++) {
        // Multiply one digit with a digit.
        mul_tmp = (bigint_ddigit_t)a.v[i] * b + mul_carry;
        mul_carry = mul_tmp >> bitsof(bigint_digit_t);

        // Accumulate the multiplier digit.
        acc_tmp = (bigint_ddigit_t)r->v[i + digit_nr] + mul_tmp + acc_carry;
        acc_carry = acc_tmp >> bitsof(bigint_digit_t);

        // Store the result.
        r->v[i + digit_nr] = (bigint_digit_t)acc_tmp;
    }
}

/** Shift left by number of digit.
 */
static inline bigint<?=$bit_size*2?>_t tvu_bigint_sld<?=$bit_size?>(bigint<?=$bit_size?>_t x, tvu_int nr_digits)
{
    bigint<?=$bit_size*2?>_t    result;
    tvu_int                     i;

    for (i = 0; i < nr_digits; i++) {
        result.v[i] = 0;
    }
    for (; i < (nr_digits + lengthof(x.v)); i++) {
        result.v[i] = x.v[i];
    }
    for (; i < lengthof(result.v); i++) {
        result.v[i] = 0;
    }
    return result;
}

/** Fused multiply add.
 */ 
static inline void tvu_bigint_fma<?=$bit_size?>(bigint<?=$bit_size*2?>_t *r, bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    tvu_int i;

    for (i = 0; i < lengthof(b.v); i++) {
        tvu_bigint_fma<?=$bit_size?>_digit(r, a, b.v[i], i);
    }
}

<?php } ?>
#endif

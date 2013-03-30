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
#include <tvutils/random.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef uint64_t    bigint_digit_t;
typedef uint128_t   bigint_ddigit_t;

<?php
$bit_sizes = array(64, 128, 256, 512, 1024, 2048, 4096);
foreach ($bit_sizes as $bit_size) {
    $nr_digits = $bit_size / 64;
?>

/** An <?=$bit_size?> integer.
 * Internally a carry is kept, which is the size of a digit.
 */
typedef struct {
    bigint_digit_t v[<?=$nr_digits?> + 1];
} bigint<?=$bit_size?>_t;

extern bigint<?=$bit_size?>_t BIGINT<?=$bit_size?>_MIN;
extern bigint<?=$bit_size?>_t BIGINT<?=$bit_size?>_ZERO;
extern bigint<?=$bit_size?>_t BIGINT<?=$bit_size?>_ONE;

<?php } ?>

<?php
foreach ($bit_sizes as $bit_size) {
    $nr_digits = $bit_size / 64;
    $first = $bit_size == $bit_sizes[0];
    $last = $bit_size == $bit_sizes[count($bit_sizes) - 1];
?>

/** Convert a hexadecimal string into a big integer.
 * @param s     String of hexadecimal digits, other characters are ignored.
 * @return      A big integer of <?=$bit_size?> bits.
 */
bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_from_hex(const utf8_t *s);

/** Convert a big integer into hexadecimal string.
 * @param x     number.
 * @return      A newly allocated hex string.
 */
utf8_t *tvu_bigint<?=$bit_size?>_to_hex(bigint<?=$bit_size?>_t x);

/** Convert a small integer into a big integer
 * @param x     Small 64 bit integer.
 * @return      A big integer of <?=$bit_size?> bits.
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_from_int(tvu_int x)
{
    int                     i;
    bigint<?=$bit_size?>_t  r;

    r.v[0] = x;
    for (i = 1; i < lengthof(r.v); i++) {
        r.v[i] = 0;
    }
    return r;
}

/** Convert a small integer into a big integer
 * @param x     Small 64 bit integer.
 * @return      A big integer of <?=$bit_size?> bits.
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_random(void)
{
    int                     i;
    bigint<?=$bit_size?>_t  r;

    // Make all digits random, except for the carry.
    for (i = 0; i < lengthof(r.v) - 1; i++) {
        r.v[i] = tvu_random();
    }
    return r;
}

/** Compare two numbers.
 * @param a     number
 * @param b     number
 * @return      Greater than 0 when a is greater than b.
 *              Smaler than 0 when a is small than b.
 *              0 when a is equal to b.
 */
static inline int tvu_bigint<?=$bit_size?>_cmp(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    tvu_int i;
    tvu_int r;

    for (i = lengthof(a.v) - 1; i >= 0; i--) {
        if (a.v > b.v) {
            return 1;
        }
        if (a.v < b.v) {
            return -1;
        }
    }
    return 0;
}

/** Greater than.
 * @param a     number
 * @param b     number
 * @return      true if a is greater than b.
 */
static inline bool tvu_bigint<?=$bit_size?>_gt(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    return tvu_bigint<?=$bit_size?>_cmp(a, b) > 0;
}

/** Is zero.
 * @param x     Number
 * @return      !x;
 */
static inline bool tvu_bigint<?=$bit_size?>_is_zero(bigint<?=$bit_size?>_t x)
{
    int                    i;

    for (i = 0; i < lengthof(x.v); i++) {
        if (x.v[i]) {
            return false;
        }
    }
    return true;
}

/** Is odd
 * @param x     Number
 * @return      x % 2;
 */
static inline bool tvu_bigint<?=$bit_size?>_is_odd(bigint<?=$bit_size?>_t x)
{
    return (x.v[0] & 1) > 0;
}

/** Bitwise not.
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_inv(bigint<?=$bit_size?>_t x)
{
    int                    i;
    bigint<?=$bit_size?>_t r;

    for (i = 0; i < lengthof(x.v); i++) {
        r.v[i] = ~x.v[i];
    }
    return r;
}

/** Negate a (positive) number.
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_neg(bigint<?=$bit_size?>_t x)
{
    int                    i;
    bigint<?=$bit_size?>_t r;
    bigint_ddigit_t        tmp;
    bigint_digit_t         carry = 1;

    for (i = 0; i < lengthof(x.v); i++) {
        tmp = (bigint_ddigit_t)~x.v[i] + carry;
        r.v[i] = (bigint_digit_t)tmp;
        carry = tmp >> bitsof(bigint_digit_t);
    }
    return r;
}

/** In place shift right.
 */
static inline void tvu_bigint<?=$bit_size?>_irshift(bigint<?=$bit_size?>_t *x, tvu_int n)
{
    int                    i;
    bigint<?=$bit_size?>_t r;
    bigint_digit_t         tmp;
    bigint_digit_t         carry = 0;

    for (i = lengthof(x->v) - 1; i >= 0; i--) {
        tmp = (x->v[i] >> n) | carry;
        carry = x->v[i] << (bitsof(bigint_digit_t) - n);
        x->v[i] = tmp;
    }
}


/** Inplace add.
 */
static inline void tvu_bigint<?=$bit_size?>_iadd(bigint<?=$bit_size?>_t *a, bigint<?=$bit_size?>_t b)
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

/** Add two number together.
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_add(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    tvu_bigint<?=$bit_size?>_iadd(&a, b);
    return a;
}

/** Accumelate subtract numbers.
 */
static inline void tvu_bigint<?=$bit_size?>_isub(bigint<?=$bit_size?>_t *a, bigint<?=$bit_size?>_t b)
{
    int                    i;
    bigint_ddigit_t        tmp;
    bigint_digit_t         carry = 1;

    for (i = 0; i < lengthof(b.v); i++) {
        tmp = (bigint_ddigit_t)a->v[i] + ((bigint_digit_t)~(b.v[i])) + carry;
        a->v[i] = (bigint_digit_t)tmp;
        carry = tmp >> bitsof(bigint_digit_t);
    }
}

/** Accumelate subtract numbers.
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_sub(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    tvu_bigint<?=$bit_size?>_isub(&a, b);
    return a;
}



<?php if (!$last) { ?>
/** Take lowest half of bits.
 * Carry is always zero.
 * @param a     number.
 * @return      a & ((1 << <?=$bit_size?>) - 1)
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_low(bigint<?=$bit_size*2?>_t a)
{
    tvu_int i;
    bigint<?=$bit_size?>_t  r;

    // Don't pick up the carry.
    for (i = 0; i < lengthof(r.v) - 1; i++) {
        r.v[i] = a.v[i];
    }
    // Clear carry.
    r.v[lengthof(r.v) - 1] = 0;
    return r;
}
<?php } ?>

<?php if (!$last) { ?>
/** Take highest half of bits.
 * @param a     number.
 * @return      a >> <?=$bit_size?>;
 */
static inline bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_high(bigint<?=$bit_size*2?>_t a)
{
    tvu_int i;
    bigint<?=$bit_size?>_t  r;

    for (i = 0; i < lengthof(r.v); i++) {
        r.v[i] = a.v[i + <?=$nr_digits?>];
    }
    return r;
}
<?php } ?>

<?php if (!$last) { ?>
/** Widen the number of bits.
 * @param a     number.
 * @return      a >> <?=$bit_size?>;
 */
static inline bigint<?=$bit_size*2?>_t tvu_bigint<?=$bit_size*2?>_wide(bigint<?=$bit_size?>_t a)
{
    tvu_int i;
    bigint<?=$bit_size*2?>_t  r;

    for (i = 0; i < lengthof(a.v); i++) {
        r.v[i] = a.v[i];
    }
    for (; i < lengthof(r.v); i++) {
        r.v[i] = 0;
    }
    return r;
}
<?php } ?>

<?php if (!$last) { ?>
/** Inplace fused multiply add with a small integer.
 * @param   r   r += (a * b)
 * @param   a   number
 * @param   b   number
 */
static inline void tvu_bigint<?=$bit_size*2?>_ifma_digit(bigint<?=$bit_size*2?>_t *r, bigint<?=$bit_size?>_t a, bigint_digit_t b, int digit_nr)
{
    int                       i;
    bigint_ddigit_t           mul_tmp;
    bigint_ddigit_t           acc_tmp;
    bigint_digit_t            mul_result;
    bigint_digit_t            mul_carry;
    bigint_digit_t            acc_carry;

    mul_carry = 0;
    acc_carry = 0;
    for (i = 0; i < lengthof(a.v); i++) {
        // Multiply one digit with a digit.
        mul_tmp = (bigint_ddigit_t)a.v[i] * b + mul_carry;
        mul_result = mul_tmp;
        mul_carry = mul_tmp >> bitsof(bigint_digit_t);

        // Accumulate the multiplier digit.
        acc_tmp = (bigint_ddigit_t)r->v[i + digit_nr] + mul_result + acc_carry;
        acc_carry = acc_tmp >> bitsof(bigint_digit_t);

        // Store the result.
        r->v[i + digit_nr] = (bigint_digit_t)acc_tmp;
    }
}
<?php } ?>

<?php if (!$last) { ?>
/** Inplace fused multiply add.
 * @param   r   r += (a * b)
 * @param   a   number
 * @param   b   number
 */
static inline void tvu_bigint<?=$bit_size*2?>_ifma(bigint<?=$bit_size*2?>_t *r, bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    tvu_int i;

    for (i = 0; i < lengthof(b.v); i++) {
        tvu_bigint<?=$bit_size*2?>_ifma_digit(r, a, b.v[i], i);
    }
}
<?php } ?>

<?php if (!$last) { ?>
/** Fused multiply add.
 * @param   a   number
 * @param   b   number
 * @param   c   number
 * @return      a + (b * c)
 */
static inline bigint<?=$bit_size*2?>_t tvu_bigint<?=$bit_size*2?>_fma(bigint<?=$bit_size*2?>_t a, bigint<?=$bit_size?>_t b, bigint<?=$bit_size?>_t c)
{
    tvu_bigint<?=$bit_size*2?>_ifma(&a, b, c);
    return a;
}
<?php } ?>

<?php if (!$last) { ?>
static inline bigint<?=$bit_size*2?>_t tvu_bigint<?=$bit_size*2?>_mul(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b)
{
    bigint<?=$bit_size*2?>_t r = BIGINT<?=$bit_size*2?>_ZERO;
    tvu_bigint<?=$bit_size*2?>_ifma(&r, a, b);
    return r;
}
<?php } ?>

<?php if (!$last) { ?>
/** Modulo with a constant number.
 * @param a     The divident.
 * @param b     The divisor.
 * @param br    The reciprical of the divisor, shifted left by the size of the divisor.
 *              The resulting reciprical will use the carry, this is expected.
 * @return      The rest value of the division.
 */
bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_mod(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b, bigint<?=$bit_size?>_t br);
<?php } ?>


<?php if (!$last) { ?>
/** Shift left by number of digit.
 */
static inline bigint<?=$bit_size*2?>_t tvu_bigint_sld<?=$bit_size*2?>(bigint<?=$bit_size?>_t x, tvu_int nr_digits)
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
<?php } ?>

<?php } ?>
#endif

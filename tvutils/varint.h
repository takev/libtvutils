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
#ifndef TVU_VARINT_H
#define TVU_VARINT_H

#include <tvutils/macros.h>
#include <tvutils/types.h>
#include <tvutils/compiler.h>
#include <tvutils/random.h>
#include <tvutils/assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

typedef uint64_t    varint_digit_t;
typedef uint128_t   varint_ddigit_t;

/** An variable sized integer.
 * This structure has an unknown size.
 */
typedef struct {
    uint16_t         nr_digits;
    uint16_t         max_nr_digits;
    uint16_t         scale_nr_digits;
    uint8_t          overflow;
    varint_digit_t   v[];
} varint_t;

/** Convert a hexadecimal string into a big integer.
 * @param s     String of hexadecimal digits, other characters are ignored.
 * @return      A big integer of <?=$bit_size?> bits.
 */
void tvu_varint_from_hex(varint_t *restrict self, const utf8_t *restrict s);

/** Copy a number into another number
 * @param self  number to copy into.
 * @param x     number to copy.
 */
static inline void tvu_varint_copy(varint_t *restrict self, const varint_t *restrict x)
{
    tvu_int i;
    tvu_int nr_digits = 0;

    self->overflow = 0;

    for (i = x->nr_digits - 1; i >= 0; i--) {
        self->v[i] = x->v[i];
        if (nr_digits == 0 && x->v[i] > 0) {
            nr_digits = i + 1;
        }
    }
    self->nr_digits = nr_digits;
}

/** Calculate the allocation size.
 * @param nr_digits Maximum number of digits to allocate.
 * @return          Number of bytes to allocate.
 */
static inline size_t tvu_varint_alloc_size(tvu_int nr_digits)
{
    return offsetof(varint_t, v) + nr_digits * sizeof(varint_digit_t);
}

/** Initialize the number to zero.
 * @param self      The number to initialize.
 * @param nr_digits Maximum number of digits this number can hold.
 * @return          Self, number is initialized to zero.
 */
static inline varint_t *tvu_varint_init(varint_t *restrict self, tvu_int nr_digits)
{
    self->max_nr_digits = nr_digits;
    self->scale_nr_digits = 0;
    self->nr_digits = 0;
    self->overflow = 0;
    return self;
}

/** Initialize the number to the value of another number.
 * @param self      The number to initialize.
 * @param nr_digits Maximum number of digits this number can hold.
 * @return          Self, number is initialized to zero.
 */
static inline varint_t *tvu_varint_init_and_copy(varint_t *restrict self, tvu_int nr_digits, const varint_t *restrict x)
{
    self->max_nr_digits = nr_digits;
    self->scale_nr_digits = 0;
    tvu_varint_copy(self, x);
    return self;
}

/** Initialize the number by converting hex.
 * @param self      The number to initialize.
 * @param nr_digits Maximum number of digits this number can hold.
 * @param s         Hex string to convert.
 * @return          self.
 */
static inline varint_t *tvu_varint_init_from_hex(varint_t *restrict self, tvu_int nr_digits, const utf8_t *restrict s)
{
    self->max_nr_digits = nr_digits;
    self->scale_nr_digits = 0;
    tvu_varint_from_hex(self, s);
    return self;
}

/** Allocate and initialize a number on the stack.
 * @param nr_digits     Number of digits to allocate.
 * @return              A number allocated on the stack.
 */
#define tvu_varint_alloca(nr_digits) tvu_varint_init(alloca(tvu_varint_alloc_size(nr_digits)), nr_digits)

/** Allocate and copy a number on the stack.
 * @param nr_digits     Number of digits to allocate.
 * @param x             The number to copy.
 * @return              A number allocated on the stack.
 */
#define tvu_varint_copya(nr_digits, x) tvu_varint_init_and_copy(alloca(tvu_varint_alloc_size(nr_digits)), nr_digits, x)

/** Allocate a number on the stack and convert 
 * @param nr_digits     Number of digits to allocate.
 * @param x             The number to copy.
 * @return              A number allocated on the stack.
 */
#define tvu_varint_from_hexa(s) tvu_varint_init_from_hex(alloca(tvu_varint_alloc_size((strlen(s) + 15) / 16)), (strlen(s) + 15) / 16, s)


/** Convert a big integer into hexadecimal string.
 * @param x     number.
 * @return      A newly allocated hex string.
 */
utf8_t *tvu_varint_to_hex(const varint_t *restrict x);

/** Convert a small integer into a big integer
 * @param x     Small 64 bit integer.
 * @return      A big integer of <?=$bit_size?> bits.
 */
static inline void tvu_varint_from_int(varint_t *restrict self, tvu_int x)
{
    self->overflow = 0;

    if (x == 0) {
        self->nr_digits = 0;
    } else {
        if (self->max_nr_digits < 1) {
            self->overflow = 1;
        }
        self->nr_digits = 1;
        self->v[0] = x;    
    }
}

/** Fill the whole integer as a random number.
 * @return      A big integer of <?=$bit_size?> bits.
 */
static inline void tvu_varint_random(varint_t *restrict self)
{
    tvu_int i;

    self->overflow = 0;
    self->nr_digits = 0;

    for (i = self->max_nr_digits; i >= 0; i--) {
        self->v[i] = tvu_random();
        if (self->nr_digits == 0 && self->v[i] > 0) {
            self->nr_digits = i + 1;
        }
    }
}

/** Normalize an integer to its minimum size.
 */
static inline void tvu_varint_normalize(varint_t *restrict self)
{
    tvu_int nr_digits = 0;
    tvu_int i;

    TVU_ASSERT(!self->overflow);

    for (i = self->nr_digits - 1; i >=0; i++) {
        if (nr_digits == 0 && self->v[i] > 0) {
            nr_digits = i + 1;
        }
    }
    self->nr_digits = nr_digits;
}


/** Compare two numbers.
 * @param a     number
 * @param b     number
 * @return      Greater than 0 when a is greater than b.
 *              Smaler than 0 when a is small than b.
 *              0 when a is equal to b.
 */
static inline int tvu_varint_cmp(const varint_t *restrict a, const varint_t *restrict b)
{
    tvu_int i;

    TVU_ASSERT(!a->overflow);
    TVU_ASSERT(!b->overflow);

    if (a->nr_digits > b->nr_digits) {
        return 1;
    }
    if (a->nr_digits < b->nr_digits) {
        return -1;
    }

    for (i = a->nr_digits - 1; i >= 0; i--) {
        if (a->v[i] > b->v[i]) {
            return 1;
        }
        if (a->v[i] < b->v[i]) {
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
static inline bool tvu_varint_gt(const varint_t *restrict a, const varint_t *restrict b)
{
    TVU_ASSERT(!a->overflow);
    TVU_ASSERT(!b->overflow);

    return tvu_varint_cmp(a, b) > 0;
}

/** Is zero.
 * @param x     Number
 * @return      !x;
 */
static inline bool tvu_varint_is_zero(const varint_t *restrict x)
{
    TVU_ASSERT(!x->overflow);

    return x->nr_digits == 0;
}

/** Is odd
 * @param x     Number
 * @return      x % 2;
 */
static inline bool tvu_varint_is_odd(const varint_t *restrict x)
{
    TVU_ASSERT(!x->overflow);

    if (x->nr_digits == 0) {
        return false;
    } else {
        return (x->v[0] & 1) > 0;
    }
}

/** In place shift right.
 * @param x     integer to shift right.
 * @param n     number of bits to shift right, between 0 and 64 inclusive.
 */
static inline void tvu_varint_irshift(varint_t *restrict x, tvu_int n)
{
    tvu_int                 i;
    varint_digit_t          tmp;
    varint_digit_t          carry = 0;
    tvu_int                 nr_digits = 0;

    TVU_ASSERT(!x->overflow);
    TVU_ASSERT(n >= 0 && n <= 64);

    for (i = x->nr_digits - 1; i >= 0; i--) {
        tmp = (x->v[i] >> n) | carry;
        carry = x->v[i] << (bitsof(varint_digit_t) - n);
        if (nr_digits == 0 && tmp > 0) {
            nr_digits = i + 1;
        }
        x->v[i] = tmp;
    }
    x->nr_digits = nr_digits;
}

/** In place shift right by number of digits.
 * @param x     integer to shift right.
 * @param n     number of digits to shift right.
 */
static inline void tvu_varint_irshiftd(varint_t *restrict x, tvu_int n)
{
    tvu_int                 i;
    varint_digit_t          tmp;
    tvu_int                 nr_digits = 0;

    TVU_ASSERT(!x->overflow);

    for (i = x->nr_digits - n - 1; i >= 0; i--) {
        tmp = (i + n < x->nr_digits) ? x->v[i + n] : 0;
        if (nr_digits == 0 && tmp > 0) {
            nr_digits = i + 1;
        }
        x->v[i] = tmp;
    }
    x->nr_digits = nr_digits;
}

/** Inplace add.
 */
static inline void tvu_varint_add(varint_t *c, varint_t *a, const varint_t *restrict b)
{
    tvu_int             i;
    varint_ddigit_t     tmp = 0;
    tvu_int             a_nr_digits = a->nr_digits;
    tvu_int             b_nr_digits = b->nr_digits;
    tvu_int             c_nr_digits = 0;
    tvu_int             nr_digits = TVU_MAX(a_nr_digits, b_nr_digits);

    TVU_ASSERT(!a->overflow);
    TVU_ASSERT(!b->overflow);

    nr_digits = TVU_MAX(a->nr_digits, b->nr_digits);

    for (i = 0; i < nr_digits || tmp > 0; i++) {
        tmp += i < a_nr_digits ? a->v[i] : 0;
        tmp += i < b_nr_digits ? b->v[i] : 0;

        if (likely(i < c->max_nr_digits)) {
            c->v[i] = (varint_digit_t)tmp;
            c_nr_digits = i + 1;
        } else {
            c->overflow = 1;
        }

        // Remember carry for next digit.
        tmp >>= bitsof(varint_digit_t);
    }
    c->nr_digits = c_nr_digits;
}

static inline void tvu_varint_iadd(varint_t *restrict a, const varint_t *restrict b)
{
    tvu_varint_add(a, a, b);
}

/** Subtract.
 */
static inline void tvu_varint_sub(varint_t *c, const varint_t *a, const varint_t *restrict b)
{
    tvu_int             i;
    varint_ddigit_t     tmp = 1;
    tvu_int             a_nr_digits = a->nr_digits;
    tvu_int             b_nr_digits = b->nr_digits;
    tvu_int             c_nr_digits = 0;
    tvu_int             nr_digits = TVU_MAX(a_nr_digits, b_nr_digits);

    TVU_ASSERT(!a->overflow);
    TVU_ASSERT(!b->overflow);
    TVU_ASSERT(tvu_varint_gt(a, b));

    nr_digits = TVU_MAX(a->nr_digits, b->nr_digits);

    // Unlike add, we will stop even if there is something in the carry.
    for (i = 0; i < nr_digits; i++) {
        tmp += i < a_nr_digits ? a->v[i] : 0;
        tmp += i < b_nr_digits ? ~(b->v[i]) : 0xffffffffffffffffULL;

        if (likely(i < c->max_nr_digits)) {
            c->v[i] = (varint_digit_t)tmp;
            c_nr_digits = i + 1;
        } else {
            c->overflow = 1;
        }

        // Remember carry for next digit.
        tmp >>= bitsof(varint_digit_t);
    }
    c->nr_digits = c_nr_digits;
}

static inline void tvu_varint_isub(varint_t *restrict a, const varint_t *restrict b)
{
    tvu_varint_sub(a, a, b);
}

/** Inplace fused multiply add with a small integer.
 * @param r         r += ((a * b) << (digit_nr * 64))
 * @param a         number
 * @param b         small number
 * @param digit_nr  Number of digits to shift the multiply result before adding to r.
 */
static inline void tvu_varint_ifma_digit(varint_t *restrict r, const varint_t *restrict a, varint_digit_t b, int digit_nr)
{
    tvu_int         i;
    varint_ddigit_t mul_tmp = 0;
    varint_ddigit_t acc_tmp = 0;
    varint_digit_t  mul_result;
    tvu_int         a_nr_digits = a->nr_digits;
    tvu_int         r_nr_digits = r->nr_digits;
    tvu_int         nr_digits = TVU_MAX(TVU_MAX(a_nr_digits, (r_nr_digits - digit_nr)), 1);

    if (b == 0) {
        return;
    }

    TVU_ASSERT(!r->overflow);
    TVU_ASSERT(!a->overflow);
    TVU_ASSERT(a->nr_digits + digit_nr <= r->max_nr_digits);

    for (i = 0; i < nr_digits || mul_tmp > 0 || acc_tmp > 0; i++) {
        // Multiply one digit with a digit.
        mul_tmp+= (i < a_nr_digits) ? ((varint_ddigit_t)a->v[i] * b) : 0;

        mul_result = mul_tmp;
        mul_tmp >>= bitsof(varint_digit_t);

        // Accumulate the multiplier digit.
        acc_tmp+= mul_result;
        acc_tmp+= (i < r_nr_digits) ? (varint_ddigit_t)r->v[i + digit_nr] : 0;

        if (i + digit_nr < r->max_nr_digits) {
            r->v[i + digit_nr] = (varint_digit_t)acc_tmp;
            if (r->v[i + digit_nr] > 0) {
                r->nr_digits = TVU_MAX(r->nr_digits, i + digit_nr + 1);
            }
        } else {
            r->overflow = 1;
        }
        acc_tmp >>= bitsof(varint_digit_t);
    }
}

/** Inplace fused multiply add.
 * @param   r   r += (a * b)
 * @param   a   number
 * @param   b   number
 */
static inline void tvu_varint_ifma(varint_t *restrict r, const varint_t *restrict a, const varint_t *restrict b)
{
    tvu_int i;

    for (i = 0; i < b->nr_digits; i++) {
        tvu_varint_ifma_digit(r, a, b->v[i], i);
    }
}

/** Fused multiply add.
 * @param   a   number
 * @param   b   number
 * @param   c   number
 * @return      a + (b * c)
 */
static inline void tvu_varint_fma(varint_t *restrict r, const varint_t *restrict a, const varint_t *restrict b, const varint_t *restrict c)
{
    tvu_varint_copy(r, a);
    tvu_varint_ifma(r, b, c);
}

static inline void tvu_varint_mul(varint_t *restrict r, const varint_t *restrict a, const varint_t *restrict b)
{
    TVU_ASSERT(!a->overflow);
    TVU_ASSERT(!b->overflow);
    tvu_varint_from_int(r, 0);
    tvu_varint_ifma(r, a, b);
}

/** Modulo with a constant number.
 * @param r     r = x % m;
 * @param x     Number to module with.
 * @param m     The divisor.
 * @param mr    The reciprical of the divisor, shifted left by the size of the divisor.
 */
void tvu_varint_mod(varint_t *restrict r, const varint_t *restrict x, const varint_t *m, const varint_t *restrict mr);

#endif

/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <stdbool.h>
#include <stdio.h>
#include <tvutils/types.h>
#include <tvutils/string.h>
#include <tvutils/varint.h>

void tvu_varint_mod(varint_t *restrict r, const varint_t *restrict x, const varint_t *restrict m, const varint_t *restrict mr)
{
    varint_t *quotient         = tvu_varint_alloca(x->nr_digits + mr->nr_digits);
    varint_t *quotient_times_m = tvu_varint_alloca(quotient->max_nr_digits);

    // quatient = x / m;
    // mr = (1 << m->nr_digits) / m;
    // quotient = (x * mr) >> m->nr_digits;
    tvu_varint_mul(quotient, x, mr);
    tvu_varint_irshiftd(quotient, mr->scale_nr_digits);

    // quotient_times_m = (x/m) * m;
    // quotient_times_m = quotient * m;
    tvu_varint_mul(quotient_times_m, quotient, m);

    // Because of rounding, we could be off by 1, this happens 25% of the time, subtract the divider once (this should happen more than once)..
    exit(0);
    while (tvu_varint_gt(quotient_times_m, x)) {
        tvu_varint_isub(quotient_times_m, m);
    }

    // x - ((x/m) * m);
    // Now simply subtract the quatient_times_m from the original argument to get the rest value.
    tvu_varint_sub(r, x, quotient_times_m);
}

/** Modular multiplication.
 * @param r     r = (a * b) % m
 * @param a     A number.
 * @param b     A number.
 * @param m     The modulo divisor.
 * @param mr    The modulo divisor recipricant. mr = (1 << m->nr_digits) /m
 */
void tvu_varint_modmul(varint_t *restrict r, const varint_t *restrict a, const varint_t *restrict b, const varint_t *restrict m, const varint_t *restrict mr)
{
    varint_t *tmp = tvu_varint_alloca(a->nr_digits + b->nr_digits);

    tvu_varint_mul(tmp, a, b);
    tvu_varint_mod(r, tmp, m, mr);
}

void tvu_varint_imodmul(varint_t *restrict r, const varint_t *restrict b, const varint_t *restrict m, const varint_t *restrict mr)
{
    varint_t *a = tvu_varint_copya(m->nr_digits, r);

    tvu_varint_modmul(r, a, b, m, mr);
}

/** Modular exponent.
 * @param r         r = (base ** exponent) % m.
 * @param _base     A number.
 * @param _exponent A number.
 * @param m         The modulo divisor.
 * @param mr        The modulo divisor recipricant. mr = (1 << m->nr_digits) /m
 */
void tvu_varint_modexp(varint_t *restrict r, const varint_t *restrict _base, const varint_t *restrict _exponent, const varint_t *restrict m, const varint_t *restrict mr)
{
    varint_t *base     = tvu_varint_copya(m->nr_digits, _base);
    varint_t *exponent = tvu_varint_copya(_exponent->nr_digits, _exponent);

    tvu_varint_from_int(r, 1);

    while (!tvu_varint_is_zero(exponent)) {
        if (tvu_varint_is_odd(exponent)) {
            tvu_varint_imodmul(r, base, m, mr);
        }

        tvu_varint_irshift(exponent, 1);
        tvu_varint_imodmul(base, base, m, mr);
    }
}

utf8_t *tvu_varint_to_hex(const varint_t *restrict r)
{
    TVU_ASSERT(r->nr_digits <= r->max_nr_digits);

    tvu_int i;
    utf8_t tmp[18];
    utf8_t *s = malloc(r->nr_digits * 17 + 1);

    s[0] = 0;

    for (i = r->nr_digits - 1; i >=0; i--) {
        snprintf(tmp, 18, "%016llx ", r->v[i]);
        strcat(s, tmp);
    }

    return s;
}

void tvu_varint_from_hex(varint_t *restrict r, const utf8_t *restrict s)
{
    tvu_int                 i;
    tvu_int                 len = strlen(s);
    tvu_int                 digit_nr;
    tvu_int                 nibble_nr;
    tvu_int                 nibble_in_digit;
    const tvu_int           nibbles_per_digit = sizeof (varint_digit_t) * 2;
    tvu_int                 nibble;

    tvu_varint_from_int(r, 0);
    for (i = 0; i < r->max_nr_digits; i++) {
        r->v[i] = 0;
    }

    for (nibble_nr = 0, i = len - 1; i >= 0; i--) {
        if ((nibble = tvu_hex2nibble(s[i])) >= 0) {
            digit_nr        = nibble_nr / nibbles_per_digit;
            nibble_in_digit = nibble_nr % nibbles_per_digit;

            if (digit_nr < r->max_nr_digits) {
                r->v[digit_nr] |= nibble << (4 * nibble_in_digit);
                r->nr_digits = digit_nr + 1;
            } else {
                r->overflow = 1;
            }

            nibble_nr++;
        }
    }
}


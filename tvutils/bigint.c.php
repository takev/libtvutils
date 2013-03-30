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
#include <tvutils/bigint.h>

<?php
$bit_sizes = array(64, 128, 256, 512, 1024, 2048, 4096);
foreach ($bit_sizes as $bit_size) {
    $nr_digits = $bit_size / 64;
?>

bigint<?=$bit_size?>_t BIGINT<?=$bit_size?>_MIN = {.v={ <?php for ($i = 0; $i < $nr_digits + 1; $i++) { echo "0, "; } ?> } };
bigint<?=$bit_size?>_t BIGINT<?=$bit_size?>_ZERO = {.v={ <?php for ($i = 0; $i < $nr_digits + 1; $i++) { echo "0, "; } ?> } };
bigint<?=$bit_size?>_t BIGINT<?=$bit_size?>_ONE = {.v={ 1, <?php for ($i = 1; $i < $nr_digits + 1; $i++) { echo "0, "; } ?> } };

<?php } ?>


<?php
$bit_sizes = array(64, 128, 256, 512, 1024, 2048, 4096);
foreach ($bit_sizes as $bit_size) {
    $first = $bit_size == $bit_sizes[0];
    $last = $bit_size == $bit_sizes[count($bit_sizes) - 1];
    $second_to_last = $bit_size == $bit_sizes[count($bit_sizes) - 2];
?>

<?php if (!$last) { ?>
bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_mod(bigint<?=$bit_size?>_t x, bigint<?=$bit_size?>_t m, bigint<?=$bit_size?>_t mr)
{
    bigint<?=$bit_size*2?>_t x_wide = tvu_bigint<?=$bit_size*2?>_wide(x);
    bigint<?=$bit_size*2?>_t m_wide = tvu_bigint<?=$bit_size*2?>_wide(m);

    // quatient = x / m;
    // mr = (1 << <?=$bit_size?>) / m;
    // quotient = (x * mr) >> <?=$bit_size?>;
    bigint<?=$bit_size?>_t quotient = tvu_bigint<?=$bit_size?>_high(tvu_bigint<?=$bit_size*2?>_mul(x, mr));

    // quotient_times_m = (x/m) * m;
    // quotient_times_m = quotient * m;
    bigint<?=$bit_size*2?>_t quotient_times_m = tvu_bigint<?=$bit_size*2?>_mul(quotient, m);

    // Because of rounding, we could be off by 1, this happens 25% of the time, subtract the divider once (this should happen more than once)..
    while (tvu_bigint<?=$bit_size*2?>_gt(quotient_times_m, x_wide)) {
        fprintf(stderr, "one off\n");
        tvu_bigint<?=$bit_size*2?>_isub(&quotient_times_m, m_wide);
    }

    // x - ((x/m) * m);
    // Now simply subtract the quatient_times_m from the original argument to get the rest value.
    return tvu_bigint<?=$bit_size?>_low(tvu_bigint<?=$bit_size*2?>_sub(x_wide, quotient_times_m));
}
<?php } ?>

<?php if (!$last && !$second_to_last) { ?>
bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_modmul(bigint<?=$bit_size?>_t a, bigint<?=$bit_size?>_t b, bigint<?=$bit_size*2?>_t m, bigint<?=$bit_size*2?>_t mr)
{
    return tvu_bigint<?=$bit_size?>_low(tvu_bigint<?=$bit_size*2?>_mod(tvu_bigint<?=$bit_size*2?>_mul(a, b), m, mr));
}
<?php } ?>


<?php if (!$last && !$second_to_last) { ?>
bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_modexp(bigint<?=$bit_size?>_t base, bigint<?=$bit_size?>_t exponent, bigint<?=$bit_size*2?>_t m, bigint<?=$bit_size*2?>_t mr)
{
    bigint<?=$bit_size?>_t  result = BIGINT<?=$bit_size?>_ONE;

    while (!tvu_bigint<?=$bit_size?>_is_zero(exponent)) {
        if (tvu_bigint<?=$bit_size?>_is_odd(exponent)) {
            result = tvu_bigint<?=$bit_size?>_modmul(result, base, m, mr);
        }

        tvu_bigint<?=$bit_size?>_irshift(&exponent, 1);
        base = tvu_bigint<?=$bit_size?>_modmul(base, base, m, mr);
    }
    return result;
}
<?php } ?>

bigint<?=$bit_size?>_t tvu_bigint<?=$bit_size?>_from_hex(const utf8_t *s)
{
    bigint<?=$bit_size?>_t  r = BIGINT<?=$bit_size?>_ZERO;
    tvu_int                 i;
    tvu_int                 len = strlen(s);
    tvu_int                 digit_nr;
    tvu_int                 nibble_nr;
    tvu_int                 nibble_in_digit;
    const tvu_int           nibbles_per_digit = sizeof (bigint_digit_t) * 2;
    tvu_int                 nibble;

    for (nibble_nr = 0, i = len - 1; i >= 0; i--) {
        if ((nibble = tvu_hex2nibble(s[i])) >= 0) {
            digit_nr        = nibble_nr / nibbles_per_digit;
            nibble_in_digit = nibble_nr % nibbles_per_digit;

            r.v[digit_nr] |= nibble << (4 * nibble_in_digit);

            nibble_nr++;
        }
    }
    return r;
}

<?php } ?>

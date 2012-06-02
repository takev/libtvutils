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

#include <stdint.h>

<?php
$bit_sizes = array(8, 16, 32, 64);
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
?>
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
<?php } ?>

<?php
$bit_sizes = array(8, 16, 32, 64);
foreach ($bit_sizes as $bit_size) {
    $nr_bytes = $bit_size / 8;
?>
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

#endif

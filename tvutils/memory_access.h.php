#ifndef TVU_MEMORY_ACCESS_H
#define TVU_MEMORY_ACCESS_H

#include <stdint.h>
#include <tvutils/endian.h>

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
static inline <?=$_sign?>int<?=$bit_size?>_t tvu_get_<?=$endian?>_<?=$sign?><?=$bit_size?>(<?=$_sign?>int<?=$bit_size?>_t * restrict buffer)
{
#ifdef TVU_ALIGNED_ACCESS
    tvu_conf_<?=$bit_size?>_t conv;
    <? for ($byte_nr = 0; $byte_nr < $nr_bytes; $byte_nr++) { ?>
    conv.c[<?=$byte_nr?>] = buffer[<?=$byte_nr?>];
    <? } ?>
    return tvu_<?=$endian?>toh_<?=$sign?><?=$bit_size?>(conv.<?=$sign?>);
#else
    tvu_conv_<?=bit_size?>_t *conv = (tvu_conv_<?=$bit_size?>_t *)buffer;
    return tvu_<?=$endian?>toh_<?=$sign?><?=$bit_size?>(conv-><?=$sign?>);
#endif
}

<? }}} ?>

#endif

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
#ifndef TVU_DYNARG_SYSV_X86_64_H
#define TVU_DYNARG_SYSV_X86_64_H

#ifndef TVU_DYNARG_H_IN
#error "Do not manually include this file, it should be included only from tvutils/dynarg.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct {
    unsigned int    gp_offset;
    unsigned int    fp_offset;
    void            *overflow_arg_area;
    void            *reg_save_area;
} tvu_va_list_t;

#define TVU_GP_SAVE_AREA_SIZE   48
#define TVU_FP_SAVE_AREA_SIZE   128       // According to ABI this should be 256, but it isn't for OS X.
#define TVU_REG_SAVE_AREA_SIZE  (TVU_GP_SAVE_AREA_SIZE + TVU_FP_SAVE_AREA_SIZE)
#define TVU_VA_BUFFER_SIZE      (TVU_REG_SAVE_AREA_SIZE + 16 * 8)  // 16 arguments extra.

/** Create a va_list structure.
 * This adds a bit over 1024 bytes on the stack.
 *
 * @param ap    The va_list structure to setup.
 */
#define tvu_va_make(ap)\
    char            ap ## _buf[TVU_VA_BUFFER_SIZE];\
    unsigned int    ap ## _gp_offset = 0;\
    unsigned int    ap ## _fp_offset = 48;\
    unsigned int    ap ## _ov_offset = TVU_REG_SAVE_AREA_SIZE;\
    unsigned int    ap ## _num_gp;\
    unsigned int    ap ## _num_fp;\
    tvu_va_list_t   *_ ## ap = (tvu_va_list_t *)ap;\
    _ ## ap->gp_offset = 0;\
    _ ## ap->fp_offset = 48;\
    _ ## ap->overflow_arg_area = &ap ## _buf[TVU_REG_SAVE_AREA_SIZE];\
    _ ## ap->reg_save_area = ap ## _buf;

/** Create a va_list structure.
 * This function does not create a buffer on the stack.
 *
 * @param ap            The va_list structure to setup.
 * @param extern_buf    Use extern buf as the buffer.
 */
#define tvu_va_make2(ap, extern_buf)\
    char            *ap ## _buf = extern_buf;\
    unsigned int    ap ## _gp_offset = 0;\
    unsigned int    ap ## _fp_offset = 48;\
    unsigned int    ap ## _ov_offset = TVU_REG_SAVE_AREA_SIZE;\
    unsigned int    ap ## _num_gp;\
    unsigned int    ap ## _num_fp;\
    tvu_va_list_t   *_ ## ap = (tvu_va_list_t *)ap;\
    _ ## ap->gp_offset = 0;\
    _ ## ap->fp_offset = 48;\
    _ ## ap->overflow_arg_area = &ap ## _buf[TVU_REG_SAVE_AREA_SIZE];\
    _ ## ap->reg_save_area = ap ## _buf;

/** Create a va_list structure.
 * This adds a bit over 1024 bytes on the stack.
 *
 * @param ap    The va_list structure to setup.
 */
#define tvu_va_reset(ap)\
    ap ## _gp_offset = 0;\
    ap ## _fp_offset = 48;\
    ap ## _ov_offset = TVU_REG_SAVE_AREA_SIZE;\
    _ ## ap->gp_offset = 0;\
    _ ## ap->fp_offset = 48;\
    _ ## ap->overflow_arg_area = &ap ## _buf[TVU_REG_SAVE_AREA_SIZE];\
    _ ## ap->reg_save_area = ap ## _buf;

/** Add a floating point register value to a valist.
 * @param ap    va_list structure.
 * @param type  Type of the floating point value to add.
 * @param v     Value to add.
 */
#define tvu_va_set_fp(ap, type, v)\
    ap ## _num_fp = (sizeof (type) + 15) / 16;\
    if (ap ## _fp_offset > (TVU_REG_SAVE_AREA_SIZE - (ap ## _num_fp * 16))) {\
        if (sizeof (type) > 8) {\
            ap ## _ov_offset = tvu_round_up_u64(ap ## _ov_offset, 16);\
        }\
        *(type *)&ap ## _buf[ap ## _ov_offset] = v;\
        ap ## _ov_offset = tvu_round_up_u64(ap ## _ov_offset + sizeof (type), 8);\
    } else {\
        *(type *)&ap ## _buf[ap ## _fp_offset] = v;\
        ap ## _fp_offset+= ap ## _num_fp * 16;\
    }

/** Add a general purpose register value to a valist.
 * @param ap    va_list structure.
 * @param type  Type of the general purpose register value to add.
 * @param v     Value to add.
 */
#define tvu_va_set_gp(ap, type, v)\
    ap ## _num_gp = (sizeof (type) + 7) / 8;\
    if (ap ## _gp_offset > (TVU_GP_SAVE_AREA_SIZE - (ap ## _num_gp * 8))) {\
        if (sizeof (type) > 8) {\
            ap ## _ov_offset = tvu_round_up_u64(ap ## _ov_offset, 16);\
        }\
        *(type *)&ap ## _buf[ap ## _ov_offset] = v;\
        ap ## _ov_offset = tvu_round_up_u64(ap ## _ov_offset + sizeof (type), 8);\
    } else {\
        *(type *)&ap ## _buf[ap ## _gp_offset] = v;\
        ap ## _gp_offset+= ap ## _num_gp * 8;\
    }

#endif

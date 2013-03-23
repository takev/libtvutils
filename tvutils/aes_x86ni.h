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
#ifndef TVU_AES_X86NI_H
#define TVU_AES_X86NI_H

#ifndef TVU_AES_H_IN
#error "Do not manually include this file, it should be included only from tvutils/aes.h"
#endif

#include <emmintrin.h>
#include <wmmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <tvutils/types.h>

#define TVU_AES_KEY_EXPANSION(key, rcon)\
    _mm_xor_si128(\
        _mm_xor_si128(\
            _mm_shuffle_epi32(\
                _mm_aeskeygenassist_si128(key, rcon),\
                0xff\
            ),\
            _mm_slli_si128(key, 12)\
        ),\
        _mm_xor_si128(\
            _mm_xor_si128(\
                key,\
                _mm_slli_si128(key, 4)\
            ),\
            _mm_slli_si128(key, 8)\
        )\
    );\

/** Create a key schedule.
 * @param key           A key.
 * @param key_schedule  The result, 10 * 128 bits.
 */
static inline int64x2x11_t tvu_aes_key_schedule(int64x2_t key)
{
    int64x2x11_t    key_schedule;

    key_schedule.v[0]  = key;
    key_schedule.v[1]  = key = TVU_AES_KEY_EXPANSION(key, 0x01);
    key_schedule.v[2]  = key = TVU_AES_KEY_EXPANSION(key, 0x02);
    key_schedule.v[3]  = key = TVU_AES_KEY_EXPANSION(key, 0x04);
    key_schedule.v[4]  = key = TVU_AES_KEY_EXPANSION(key, 0x08);
    key_schedule.v[5]  = key = TVU_AES_KEY_EXPANSION(key, 0x10);
    key_schedule.v[6]  = key = TVU_AES_KEY_EXPANSION(key, 0x20);
    key_schedule.v[7]  = key = TVU_AES_KEY_EXPANSION(key, 0x40);
    key_schedule.v[8]  = key = TVU_AES_KEY_EXPANSION(key, 0x80);
    key_schedule.v[9]  = key = TVU_AES_KEY_EXPANSION(key, 0x1b);
    key_schedule.v[10] = key = TVU_AES_KEY_EXPANSION(key, 0x36);
    return key_schedule;
}

static inline int64x2x8_t tvu_aes_white_wash(int64x2x8_t t, int64x2_t key, int nr_blocks)
{
    switch (nr_blocks) {
    case 8: t.v[7] = _mm_xor_si128(t.v[7], key);
    case 7: t.v[6] = _mm_xor_si128(t.v[6], key);
    case 6: t.v[5] = _mm_xor_si128(t.v[5], key);
    case 5: t.v[4] = _mm_xor_si128(t.v[4], key);
    case 4: t.v[3] = _mm_xor_si128(t.v[3], key);
    case 3: t.v[2] = _mm_xor_si128(t.v[2], key);
    case 2: t.v[1] = _mm_xor_si128(t.v[1], key);
    case 1: t.v[0] = _mm_xor_si128(t.v[0], key);
    }
    return t;
}

static inline int64x2x8_t tvu_aes_encrypt_round(int64x2x8_t t, int64x2_t key, int nr_blocks)
{
    switch (nr_blocks) {
    case 8: t.v[7] = _mm_aesenc_si128(t.v[7], key);
    case 7: t.v[6] = _mm_aesenc_si128(t.v[6], key);
    case 6: t.v[5] = _mm_aesenc_si128(t.v[5], key);
    case 5: t.v[4] = _mm_aesenc_si128(t.v[4], key);
    case 4: t.v[3] = _mm_aesenc_si128(t.v[3], key);
    case 3: t.v[2] = _mm_aesenc_si128(t.v[2], key);
    case 2: t.v[1] = _mm_aesenc_si128(t.v[1], key);
    case 1: t.v[0] = _mm_aesenc_si128(t.v[0], key);
    }
    return t;
}

static inline int64x2x8_t tvu_aes_encrypt_last_round(int64x2x8_t t, int64x2_t key, int nr_blocks)
{
    switch (nr_blocks) {
    case 8: t.v[7] = _mm_aesenclast_si128(t.v[7], key);
    case 7: t.v[6] = _mm_aesenclast_si128(t.v[6], key);
    case 6: t.v[5] = _mm_aesenclast_si128(t.v[5], key);
    case 5: t.v[4] = _mm_aesenclast_si128(t.v[4], key);
    case 4: t.v[3] = _mm_aesenclast_si128(t.v[3], key);
    case 3: t.v[2] = _mm_aesenclast_si128(t.v[2], key);
    case 2: t.v[1] = _mm_aesenclast_si128(t.v[1], key);
    case 1: t.v[0] = _mm_aesenclast_si128(t.v[0], key);
    }
    return t;
}

/** Create a counter block.
 * @param nonce     A one-time value transfered in the packet header.
 * @param counter   A value incremented for each encryption block.
 * @return          A block of counter/nonce values to encrypt.
 */
static inline int64x2x8_t tvu_aes_ctr_create_counters(uint64_t nonce, int64_t counter, int nr_blocks)
{
    int64x2x8_t       dst;
    int64x2_t         ctr = {counter + nr_blocks, nonce};
    const int64x2_t   ONE_ZERO = {1, 0};
    
    switch (nr_blocks) {
    case 8: dst.v[7] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    case 7: dst.v[6] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    case 6: dst.v[5] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    case 5: dst.v[4] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    case 4: dst.v[3] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    case 3: dst.v[2] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    case 2: dst.v[1] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    case 1: dst.v[0] = ctr = _mm_sub_epi64(ctr, ONE_ZERO);
    }
    return dst;
}

/** Scramble plain text into cypher text.
 * The scamble is a simple xor of the plain text with a one-time-pad.
 * This function does not directly store the cypher text since, a galois counter could be added.
 *
 * @param pad           A 1024 bit one-time-pad created by encrypting a counter.
 * @param src           A 1024 bit text to (de)scramble.
 * @param nr_blocks     Number of 128 bit blocks of plain text to scramble.
 * @return              A 1024 bit (de)scambled text.
 */
static inline int64x2x8_t tvu_aes_ctr_scramble(int64x2x8_t pad, int64x2x8_t src, int nr_blocks)
{
    int64x2x8_t dst;

    switch (nr_blocks) {
    case 8: dst.v[7] = _mm_xor_si128(pad.v[7], src.v[7]);
    case 7: dst.v[6] = _mm_xor_si128(pad.v[6], src.v[6]);
    case 6: dst.v[5] = _mm_xor_si128(pad.v[5], src.v[5]);
    case 5: dst.v[4] = _mm_xor_si128(pad.v[4], src.v[4]);
    case 4: dst.v[3] = _mm_xor_si128(pad.v[3], src.v[3]);
    case 3: dst.v[2] = _mm_xor_si128(pad.v[2], src.v[2]);
    case 2: dst.v[1] = _mm_xor_si128(pad.v[1], src.v[1]);
    case 1: dst.v[0] = _mm_xor_si128(pad.v[0], src.v[0]);
    }
    return dst;
}

/** Store SSE registers into memory.
 * @param dst       128 bit aligned destination memory, up to 1024 bits in size.
 * @param src       1024 bit SSE registers.
 * @param nr_blocks Number of 128 bit blocks to store.
 */
static inline void tvu_aes_block_store(int64x2_t *dst, int64x2x8_t src, int nr_blocks)
{
    switch (nr_blocks) {
    case 8: dst[7] = src.v[7];
    case 7: dst[6] = src.v[6];
    case 6: dst[5] = src.v[5];
    case 5: dst[4] = src.v[4];
    case 4: dst[3] = src.v[3];
    case 3: dst[2] = src.v[2];
    case 2: dst[1] = src.v[1];
    case 1: dst[0] = src.v[0];
    }
}

/** Load SSE registers from memory.
 * @param src       128 bit aligned source memory, up to 1024 bits in size.
 * @param nr_blocks Number of 128 bit blocks to store.
 * @return          1024 bit SSE registers.
 */
static inline int64x2x8_t tvu_aes_block_load(int64x2_t *src, int nr_blocks)
{
    int64x2x8_t dst;

    switch (nr_blocks) {
    case 8: dst.v[7] = src[7];
    case 7: dst.v[6] = src[6];
    case 6: dst.v[5] = src[5];
    case 5: dst.v[4] = src[4];
    case 4: dst.v[3] = src[3];
    case 3: dst.v[2] = src[2];
    case 2: dst.v[1] = src[1];
    case 1: dst.v[0] = src[0];
    }
    return dst;
}


#endif

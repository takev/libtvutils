
#ifndef TVU_INTEGER_H
#define TVU_INTEGER_H
#define TVU_INTEGER_H_IN

#include <stdint.h>
#include <tvutils/integer_model.h>

typedef union {
    char        c[1];
    uint8_t     u;
    int8_t      i;
} tvu_conv_8_t;

typedef union {
    char        c[2];
    uint16_t    u;
    int16_t     i;
} tvu_conv_16_t;

typedef union {
    char        c[4];
    uint32_t    u;
    int32_t     i;
    float       f;
} tvu_conv_32_t;

typedef union {
    char        c[8];
    uint64_t    u;
    int64_t     i;
    double      f;
} tvu_conv_64_t;

/*
typedef union {
    char        c[16];
    uint128_t   u;
    int128_t    i;
} tvu_conv_128_t;
*/

/** Sign extend a integer.
 * The integer of 'b' bits is stored in x. A signed integer is returned.
 * @param x     The integer to sign extend.
 * @param b     The number of bits of the integer.
 * @returns     The signed integer.
 */
static inline int64_t tvu_sign_extend(uint64_t x, unsigned int b)
{
    unsigned int m = (sizeof (x) * 8) - b;
    return (int64_t)(x << m) >> m;
}

#ifdef TVU_ALIGNED_ACCESS
#include <tvutils/integer_aligned.h>
#else
#include <tvutils/integer_unaligned.h>
#endif

#undef TVU_INTEGER_H_IN
#endif

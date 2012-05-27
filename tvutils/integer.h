
#ifndef TVU_INTEGER_H
#define TVU_INTEGER_H
#define TVU_INTEGER_H_IN

#include <stdint.h>

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

#endif

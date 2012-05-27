#ifndef TVU_ENDIAN_BSD_H
#define TVU_ENDIAN_BSD_H

#ifndef TVU_ENDIAN_H_IN
#error "Do not manually include this file, it should be included only from tvutils/endian.h"
#endif

#include <stdint.h>
#include <machine/endian.h>
#include <machine/byte_order.h>

#if BYTE_ORDER == BIG_ENDIAN
#define TVU_BIG_ENDIAN
#endif

static inline uint16_t tvu_betoh_u16(uint16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return OSSwapInt16(x);
#endif
}

static inline int16_t tvu_betoh_i16(int16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return OSSwapInt16(x);
#endif
}

static inline uint32_t tvu_betoh_u32(uint32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return OSSwapInt32(x);
#endif
}

static inline int32_t tvu_betoh_i32(int32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return OSSwapInt32(x);
#endif
}

static inline uint64_t tvu_betoh_u64(uint64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return OSSwapInt64(x);
#endif
}

static inline int64_t tvu_betoh_i64(int64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return OSSwapInt64(x);
#endif
}


#endif

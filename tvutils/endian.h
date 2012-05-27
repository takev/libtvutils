#ifndef TVU_ENDIAN_BSD_H
#define TVU_ENDIAN_BSD_H

#if __GNUC_LIBRARY__ >= 6
#error "Could not find an endian implementation for glibc platform."

#elif defined(__APPLE__)
#include <stdint.h>
#include <machine/endian.h>
#include <machine/byte_order.h>
#define tvu_swap_i8(x)  (x)
#define tvu_swap_i16(x) OSSwapInt16(x)
#define tvu_swap_i32(x) OSSwapInt32(x)
#define tvu_swap_i64(x) OSSwapInt64(x)
#define tvu_swap_u8(x)  (x)
#define tvu_swap_u16(x) OSSwapInt16(x)
#define tvu_swap_u32(x) OSSwapInt32(x)
#define tvu_swap_u64(x) OSSwapInt64(x)

#else
#error "Could not find an endian implementation for this platform."
#endif

/** Convert a little endian 8-bit signed integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline int8_t tvu_letoh_i8(int8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i8(x);
#else
    return x;
#endif
}

/** Convert a native 8-bit signed integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline int8_t tvu_htole_i8(int8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i8;
#else
    return x;
#endif
}

/** Convert a little endian 8-bit unsigned integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint8_t tvu_letoh_u8(uint8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u8(x);
#else
    return x;
#endif
}

/** Convert a native 8-bit unsigned integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline uint8_t tvu_htole_u8(uint8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u8;
#else
    return x;
#endif
}

/** Convert a big endian 8-bit signed integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline int8_t tvu_betoh_i8(int8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i8(x);
#endif
}

/** Convert a native 8-bit signed integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline int8_t tvu_htobe_i8(int8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i8(x);
#endif
}

/** Convert a big endian 8-bit unsigned integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint8_t tvu_betoh_u8(uint8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u8(x);
#endif
}

/** Convert a native 8-bit unsigned integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline uint8_t tvu_htobe_u8(uint8_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u8(x);
#endif
}

/** Convert a little endian 16-bit signed integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline int16_t tvu_letoh_i16(int16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i16(x);
#else
    return x;
#endif
}

/** Convert a native 16-bit signed integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline int16_t tvu_htole_i16(int16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i16;
#else
    return x;
#endif
}

/** Convert a little endian 16-bit unsigned integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint16_t tvu_letoh_u16(uint16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u16(x);
#else
    return x;
#endif
}

/** Convert a native 16-bit unsigned integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline uint16_t tvu_htole_u16(uint16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u16;
#else
    return x;
#endif
}

/** Convert a big endian 16-bit signed integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline int16_t tvu_betoh_i16(int16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i16(x);
#endif
}

/** Convert a native 16-bit signed integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline int16_t tvu_htobe_i16(int16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i16(x);
#endif
}

/** Convert a big endian 16-bit unsigned integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint16_t tvu_betoh_u16(uint16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u16(x);
#endif
}

/** Convert a native 16-bit unsigned integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline uint16_t tvu_htobe_u16(uint16_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u16(x);
#endif
}

/** Convert a little endian 32-bit signed integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline int32_t tvu_letoh_i32(int32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i32(x);
#else
    return x;
#endif
}

/** Convert a native 32-bit signed integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline int32_t tvu_htole_i32(int32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i32;
#else
    return x;
#endif
}

/** Convert a little endian 32-bit unsigned integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint32_t tvu_letoh_u32(uint32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u32(x);
#else
    return x;
#endif
}

/** Convert a native 32-bit unsigned integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline uint32_t tvu_htole_u32(uint32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u32;
#else
    return x;
#endif
}

/** Convert a big endian 32-bit signed integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline int32_t tvu_betoh_i32(int32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i32(x);
#endif
}

/** Convert a native 32-bit signed integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline int32_t tvu_htobe_i32(int32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i32(x);
#endif
}

/** Convert a big endian 32-bit unsigned integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint32_t tvu_betoh_u32(uint32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u32(x);
#endif
}

/** Convert a native 32-bit unsigned integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline uint32_t tvu_htobe_u32(uint32_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u32(x);
#endif
}

/** Convert a little endian 64-bit signed integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline int64_t tvu_letoh_i64(int64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i64(x);
#else
    return x;
#endif
}

/** Convert a native 64-bit signed integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline int64_t tvu_htole_i64(int64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_i64;
#else
    return x;
#endif
}

/** Convert a little endian 64-bit unsigned integer to host native.
 * @param x     little endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint64_t tvu_letoh_u64(uint64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u64(x);
#else
    return x;
#endif
}

/** Convert a native 64-bit unsigned integer to little endian.
 * @param x     Host native integer.
 * @return      little endian integer.
 */
static inline uint64_t tvu_htole_u64(uint64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return tvu_swap_u64;
#else
    return x;
#endif
}

/** Convert a big endian 64-bit signed integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline int64_t tvu_betoh_i64(int64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i64(x);
#endif
}

/** Convert a native 64-bit signed integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline int64_t tvu_htobe_i64(int64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_i64(x);
#endif
}

/** Convert a big endian 64-bit unsigned integer to host native.
 * @param x     big endian integer to convert to native.
 * @return      Native integer.
 */
static inline uint64_t tvu_betoh_u64(uint64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u64(x);
#endif
}

/** Convert a native 64-bit unsigned integer to big endian.
 * @param x     Host native integer.
 * @return      big endian integer.
 */
static inline uint64_t tvu_htobe_u64(uint64_t x)
{
#ifdef TVU_BIG_ENDIAN
    return x;
#else
    return tvu_swap_u64(x);
#endif
}


#endif

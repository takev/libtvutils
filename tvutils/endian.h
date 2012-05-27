#ifndef TVU_ENDIAN_H
#define TVU_ENDIAN_H
#define TVU_ENDIAN_H_IN

#include <stdlib.h>

#if __GNUC_LIBRARY__ >= 6
#include <tvutils/endian_glibc.h>

#elif defined(__APPLE__)
#include <tvutils/endian_bsd.h>

#else
#error "Could not find an endian implementation for this platform."
#endif

#undef TVU_ENDIAN_H_IN
#endif

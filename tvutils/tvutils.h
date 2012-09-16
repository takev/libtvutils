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
#ifndef TVU_UTILS_H
#define TVU_UTILS_H

/** \mainpage Take Vos's Library of useful utilities.
 * This library will grow as I add new utilities which will not fit in another
 * library. This started with the libraries like libuniqueid and libmarshall
 * which included things that really didn't fit and overlap.
 *
 * \section atomic Atomic operations
 * Atomic operations can be found in the atomic_gcc.h header file. atomic.h will
 * include this or another platform's implementation. These functions are atomic
 * operations which are platform independed. Currently
 * only gcc/llvm intrinsics are used, OS X used to have different operations for this.
 *
 * For easy of use and compatibility these operations are made for each data type.
 * 16, 32, 64 bits; signed and unsigned integers.
 *
 * \section time Time manipulation
 * The time manipulation functions can be found in time_posix1993.h or time_bsd.h
 * which are included from time.h. For easy time delta calculations time is stored
 * as a 64 bit 32.32 fixed point; the number of seconds since January 1st 2012.
 * Simple add and subtract operation on the 64 bit signed integer yield the correct
 * result. The value of 1 in this integer means 1/(2^32) seconds.
 *
 * \section endian Endian swapping
 * There seem to be no standard for endian swapping; it is different for bsd, Linux
 * Windows, etc. The endian swapping in socket.h does not have a 64 bit variant.
 * The functions in endian.h can handle swapping from and to host native to and from
 * big endian and little endian for 8, 16, 32 and 64 bit signed and unsigned integers.
 *
 * \section memory_access Memory access
 * Taking an integer or float value from a byte array has pitfalls. The memory_access.h
 * file tries to solve most problems, including big- and little-endian conversion,
 * aligned or unaligned memory access and strict aliasing.
 *
 * \section types Standard types
 * types.h just defines float32_t, float64_t and float80_t to mirror what has been done for
 * the explicit integer definitions in stdint.h
 * It also defines the standard types: tvu_int and tvu_float.
 *
 * \section integer Integer utilities
 * iteger.h Includes a collection of functions operating on integers.
 */

#include <tvutils/types.h>
#include <tvutils/common.h>
#include <tvutils/path.h>
#include <tvutils/target.h>
#include <tvutils/compiler.h>
#include <tvutils/assert.h>
#include <tvutils/macros.h>
#include <tvutils/buffer.h>
#include <tvutils/time.h>
#include <tvutils/atomic.h>
#include <tvutils/dynarg.h>
#include <tvutils/number.h>
#include <tvutils/string.h>
#include <tvutils/endian.h>
#include <tvutils/memory_access.h>
#include <tvutils/ringbuffer.h>
#include <tvutils/unit_test.h>
#include <tvutils/daemon.h>
#include <tvutils/base64.h>
#include <tvutils/random.h>
#include <tvutils/hash.h>
#include <tvutils/permhashtable.h>
#include <tvutils/iopoll.h>

#endif

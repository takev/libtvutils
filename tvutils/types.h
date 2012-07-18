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
#ifndef TVU_TYPES_H
#define TVU_TYPES_H

#include <stdint.h>

typedef float       float32_t;
typedef double      float64_t;
typedef long double float80_t;

/** Standard float type.
 * Use this type anywhere you need a non specific sized float.
 */
typedef float64_t   tvu_float;

/** Standard int type.
 * Use this type anywhere you need a non specific sized int.
 */
typedef int64_t     tvu_int;

/** Standard UTF-t string type.
 */
typedef char     utf8_t;

#endif

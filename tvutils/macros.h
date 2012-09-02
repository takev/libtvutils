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
#ifndef TVU_MACROS_H
#define TVU_MACROS_H

#define TVU_MAX(a, b)       ((a) > (b) ? (a) : (b))
#define TVU_MIN(a, b)       ((a) < (b) ? (a) : (b))
#define TVU_UNSIGNED(a)     ((a) > 0  ? (a) : 0)
#define TVU_CLAMP(a)        ((a) > -1 ? ((a < 1) ? (a) : 1) : -1)
#define TVU_ABS(a)          ((a) >= 0 ? (a) : -(a))

/** Increment value and wrap around.
 * @param a     value to increment
 * @param n     number of values, beyond value wraps to 0.
 * @returns     incremented values, or wrapped to 0.
 */
#define TVU_INCWRAP(a, n)   (((a) + 1) < (n) ? ((a) + 1) : 0)


#endif

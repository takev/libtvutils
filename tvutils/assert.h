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
#ifndef TVU_ASSERT_H
#define TVU_ASSERT_H

#include <stdlib.h>
#include <tvutils/compiler.h>

#define TVU_ASSERT(x)\
    if (unlikely(!(x))) {\
        fprintf(stderr, "ASSERT FAILED %s:%i '%s'\n", __FILE__, (int)__LINE__, #x);\
        abort();\
    }

#endif
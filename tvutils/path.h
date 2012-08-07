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
#ifndef TVU_PATH_H
#define TVU_PATH_H

#include <stdlib.h>
#include <limits.h>
#include <tvutils/types.h>

/** The name of the program without the path.
 */
extern utf8_t  tvu_program_name[PATH_MAX];

/** The path + program as comming from argument 0.
 */
extern utf8_t  tvu_program_path[PATH_MAX];

/** The location where the program is located.
 */
extern utf8_t  tvu_program_dir[PATH_MAX];

/** Parse argument 0 for the program name and path.
 * @param argv0     argument 0
 */
void tvu_path_init(utf8_t *argv0);

#endif

/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <stdlib.h>
#include <libgen.h>
#include <strings.h>
#include <tvutils/path.h>

utf8_t  tvu_program_name[PATH_MAX];
utf8_t  tvu_program_path[PATH_MAX];
utf8_t  tvu_program_dir[PATH_MAX];

void tvu_path_init(utf8_t *argv0)
{
    utf8_t  tmp[PATH_MAX];

    strcpy(tvu_program_path, argv0);
    strcpy(tvu_program_name, basename(argv0));
    realpath(argv0, tmp);
    strcpy(tvu_program_dir, dirname(tmp));
}




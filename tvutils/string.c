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
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <tvutils/string.h>

void tvu_perror(utf8_t const * restrict fmt, ...)
{
    utf8_t  *tmp_s;
    int     tmp_errno = errno;
    va_list ap;

    va_start(ap, fmt);
    vasprintf(&tmp_s, fmt, ap);
    va_end(ap);

    fprintf(stderr, "%s: %s\n", tmp_s, strerror(tmp_errno));
    free(tmp_s);
}


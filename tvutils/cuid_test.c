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
#include <stdio.h>
#include <tvutils/tvutils.h>
#include <tvutils/cuid.h>

int main(int argc __attribute__((unused)), char *argv[])
{
    tvu_cuid_t   r;
    tvu_cuid_t   prev_r;
    int         i;

    TVU_UNITTEST_INIT
    tvu_init(argv[0]);

    // Loop 16 times to the buffer.
    prev_r = tvu_random();
    for (i = 0; i < 0x10000; i++) {
        r = tvu_cuid();
        TVU_UNITTEST_NOT_CMP(r, prev_r)
        prev_r = r;
    }

    return 0;
}

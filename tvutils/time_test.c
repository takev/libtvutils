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
#include <tvutils/time.h>

int main(int argc __attribute__((unused)), char *argv[])
{
    utf8_t  buf[256];
    time_t  t1;
    time_t  t2;

    TVU_UNITTEST_INIT
    tvu_init(argv[0]);

    // Check if formating a simple timestamp works.
    tvu_fmt_timestamp(buf, 0);
    TVU_UNITTEST_STRCMP(buf, "2012-01-01 00:00:00.000000000");

    tvu_fmt_datetime(buf, 0x112ac6c00000001);
    TVU_UNITTEST_STRCMP(buf, "2012-07-27 08:16:44");

    tvu_fmt_timestamp(buf, 0x112ac6c00000005);
    TVU_UNITTEST_STRCMP(buf, "2012-07-27 08:16:44.000000001");

    // Check if the time from our clock, is somewhat equal to the OS clock.
    t1 = tvu_to_timet(tvu_time());
    t2 = time(NULL);
    TVU_UNITTEST_EPSILON_CMP(t1, t2, 2);

    return 0;
}

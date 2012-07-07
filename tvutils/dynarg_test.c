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
#include <tvutils/dynarg.h>


int test1(void)
{
    TVU_UNITTEST_INIT

    va_list ap;
    char    s[128];

    tvu_va_make(ap);

    tvu_va_reset(ap);
    tvu_va_set_gp(ap, int, 10);
    tvu_va_set_gp(ap, int, 20);
    vsprintf(s, "%i,%i", ap);
    TVU_UNITTEST_STRCMP(s, "10,20");

    tvu_va_reset(ap);
    tvu_va_set_fp(ap, double, 10.0);
    tvu_va_set_fp(ap, double, 20.0);
    vsprintf(s, "%.1f,%.1f", ap);
    TVU_UNITTEST_STRCMP(s, "10.0,20.0");

    tvu_va_reset(ap);
    tvu_va_set_gp(ap, int, 1);
    tvu_va_set_gp(ap, int, 2);
    tvu_va_set_gp(ap, int, 3);
    tvu_va_set_gp(ap, int, 4);
    tvu_va_set_gp(ap, int, 5);
    tvu_va_set_gp(ap, int, 6);
    tvu_va_set_gp(ap, int, 7);  // This should be in overflow
    tvu_va_set_gp(ap, int, 8);  // This should be in overflow
    vsprintf(s, "%i,%i,%i,%i,%i,%i,%i,%i", ap);
    TVU_UNITTEST_STRCMP(s, "1,2,3,4,5,6,7,8");

    tvu_va_reset(ap);
    tvu_va_set_fp(ap, double, 1.0);
    tvu_va_set_fp(ap, double, 2.0);
    tvu_va_set_fp(ap, double, 3.0);
    tvu_va_set_fp(ap, double, 4.0);
    tvu_va_set_fp(ap, double, 5.0);
    tvu_va_set_fp(ap, double, 6.0);
    tvu_va_set_fp(ap, double, 7.0);
    tvu_va_set_fp(ap, double, 8.0);
    tvu_va_set_fp(ap, double, 9.0);
    tvu_va_set_fp(ap, double, 10.0);
    tvu_va_set_fp(ap, double, 11.0);
    tvu_va_set_fp(ap, double, 12.0);
    tvu_va_set_fp(ap, double, 13.0);
    tvu_va_set_fp(ap, double, 14.0);
    tvu_va_set_fp(ap, double, 15.0);
    tvu_va_set_fp(ap, double, 16.0);
    tvu_va_set_fp(ap, double, 17.0);
    tvu_va_set_fp(ap, double, 18.0);
    vsprintf(s, "%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f", ap);
    TVU_UNITTEST_STRCMP(s, "1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0");

    return 0;
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    int r;

    tvu_init();

    if ((r = test1()) != 0) {
        return r;
    }

    return 0; 
}

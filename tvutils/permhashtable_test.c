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

char                buffer[1024];
tvu_permhashtable_t *table = (tvu_permhashtable_t *)buffer;

int main(int argc __attribute__((unused)), char *argv[])
{
    utf8_t const *result;

    TVU_UNITTEST_INIT
    tvu_init(argv[0]);

    memset(buffer, 0, sizeof (buffer));
    tvu_permhashtable_init(table, sizeof (buffer), 16, 16);

    result = tvu_permhashtable_set_s_s(table, "hello", "world");
    TVU_UNITTEST_NOT_NULL(result)
    TVU_UNITTEST_STRCMP(result, "world")

    result = tvu_permhashtable_set_s_s(table, "hallo", "wereld");
    TVU_UNITTEST_NOT_NULL(result)
    TVU_UNITTEST_STRCMP(result, "wereld")

    result = tvu_permhashtable_set_s_s(table, "hallo", "wereld");
    TVU_UNITTEST_NOT_NULL(result)
    TVU_UNITTEST_STRCMP(result, "wereld")

    result = tvu_permhashtable_set_s_s(table, "hallo", "world");
    TVU_UNITTEST_NULL(result)

    result = tvu_permhashtable_get_s_s(table, "hallo");
    TVU_UNITTEST_NOT_NULL(result)
    TVU_UNITTEST_STRCMP(result, "wereld")

    result = tvu_permhashtable_get_s_s(table, "hello");
    TVU_UNITTEST_NOT_NULL(result)
    TVU_UNITTEST_STRCMP(result, "world")

    result = tvu_permhashtable_get_s_s(table, "yo");
    TVU_UNITTEST_NULL(result)
    

    return 0;
}

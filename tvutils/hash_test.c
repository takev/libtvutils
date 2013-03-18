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
#include <stdio.h>
#include <tvutils/tvutils.h>
#include <tvutils/hash.h>

int main(int argc __attribute__((unused)), char *argv[])
{
    tvu_hash_t  hash;
    int         i;

    TVU_UNITTEST_INIT
    tvu_init(argv[0]);

    hash = tvu_hash_string("hello world");
    TVU_UNITTEST_UCMP(hash, 13360386925911296650ULL);

    hash = tvu_hash_string("You should have received a copy of the GNU General Public License");
    TVU_UNITTEST_UCMP(hash, 17944286236647688188ULL);

    hash = tvu_hash_string("GNU");
    TVU_UNITTEST_UCMP(hash, 8579761664830348054ULL);

    hash = tvu_hash((uint8_t const *)"GNU\0\0", 3);
    TVU_UNITTEST_UCMP(hash, 8579761664830348054ULL);
    hash = tvu_hash((uint8_t const *)"GNU\0\0", 4);
    TVU_UNITTEST_UCMP(hash, 2224014147481998463ULL);
    hash = tvu_hash((uint8_t const *)"GNU\0\0", 5);
    TVU_UNITTEST_UCMP(hash, 16609359652666880915ULL);
    return 0;
}

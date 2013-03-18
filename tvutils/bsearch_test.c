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
#include <tvutils/bsearch.h>

typedef struct {
    uint64_t    key;
    uint8_t     index;
} test_entry_t; 

test_entry_t test_table[] = {
    {1, 0}, {2, 1}, {4, 2}, {5, 3}, {5, 4}, {7, 5}
};

int search_test_compar(const void *_key, const void *_entry)
{
    const uint64_t        *key = _key;
    const test_entry_t    *entry = _entry;

    return *key - entry->key;
}

uint8_t search_test(uint64_t key)
{
    test_entry_t *entry;

    entry = tvu_bsearch_near(&key, test_table, sizeof (test_table) / sizeof (test_entry_t), sizeof (test_entry_t), search_test_compar);
    return entry->index;
}

int main(int argc __attribute__((unused)), char *argv[])
{
    TVU_UNITTEST_INIT

    uint8_t r;

    tvu_init(argv[0]);

    r = search_test(0);
    TVU_UNITTEST_CMP(r, 0);

    r = search_test(1);
    TVU_UNITTEST_CMP(r, 0);

    r = search_test(2);
    TVU_UNITTEST_CMP(r, 1);

    r = search_test(3);
    TVU_UNITTEST_CMP(r, 1);

    r = search_test(4);
    TVU_UNITTEST_CMP(r, 2);

    r = search_test(5);
    TVU_UNITTEST_CMP(r, 3);

    r = search_test(6);
    TVU_UNITTEST_CMP(r, 4);

    r = search_test(7);
    TVU_UNITTEST_CMP(r, 5);

    r = search_test(8);
    TVU_UNITTEST_CMP(r, 5);

    return 0;
}

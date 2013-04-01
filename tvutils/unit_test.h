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
#ifndef TVU_UNIT_TEST_H
#define TVU_UNIT_TEST_H

#include <stdio.h>
#include <tvutils/macros.h>
#include <tvutils/varint.h>

#define TVU_UNITTEST_INIT\
    char tvu_unittest_fmt[128];

#define TVU_UNITTEST_MEMCMP(d1, d2, n)\
    if (memcmp(d1, d2, n) != 0) {\
        utf8_t *s1 = tvu_voidp2hex(d1, n);\
        utf8_t *s2 = tvu_voidp2hex(d2, n);\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: \n%s\n", s2);\
        fprintf(stderr, "Got: \n%s\n", s1);\
        fprintf(stderr, "----------------\n");\
        free(s1);\
        free(s2);\
        return 1;\
    }

#define TVU_UNITTEST_STRCMP(s1, s2)\
    if (strcmp(s1, s2) != 0) {\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: '%s'\n", s2);\
        fprintf(stderr, "Got:      '%s'\n", s1);\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_CMP(a, b)\
    if (a != b) {\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: %lli\n", (long long)b);\
        fprintf(stderr, "Got:      %lli\n", (long long)a);\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_UCMP(a, b)\
    if (a != b) {\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: %llu\n", (unsigned long long)b);\
        fprintf(stderr, "Got:      %llu\n", (unsigned long long)a);\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_NOT_CMP(a, b)\
    if (a == b) {\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Not Expecting: %lli\n", (long long)b);\
        fprintf(stderr, "Got:           %lli\n", (long long)a);\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_EPSILON_CMP(a, b, e)\
    if (TVU_ABS(a - b) > e) {\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: %lli\n", (long long)b);\
        fprintf(stderr, "Got:      %lli\n", (long long)a);\
        fprintf(stderr, "Difference of %lli, larger than epsilon (%lli)\n", (long long)TVU_ABS(a - b), (long long)e);\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_NULL(a)\
    if (a != NULL) {\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: NULL\n");\
        fprintf(stderr, "Got:      %llx\n", (unsigned long long)((char *)a - (char *)NULL));\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_NOT_NULL(a)\
    if (a == NULL) {\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: NOT NULL\n");\
        fprintf(stderr, "Got:      NULL\n");\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_VARINTCMP(d1, d2)\
    if (tvu_varint_cmp(d1, d2) != 0) {\
        utf8_t *s1 = tvu_varint_to_hex(d1);\
        utf8_t *s2 = tvu_varint_to_hex(d2);\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, "Expected: \n%s\n", s2);\
        fprintf(stderr, "Got: \n%s\n", s1);\
        fprintf(stderr, "----------------\n");\
        free(s1);\
        free(s2);\
        return 1;\
    }


#endif

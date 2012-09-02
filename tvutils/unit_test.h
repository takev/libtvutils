#ifndef TVU_UNIT_TEST_H
#define TVU_UNIT_TEST_H

#include <stdio.h>
#include <tvutils/macros.h>

#define TVU_UNITTEST_INIT\
    char tvu_unittest_fmt[128];

#define TVU_UNITTEST_MEMCMP(s1, s2, n)\
    if (memcmp(s1, s2, n) != 0) {\
        snprintf(tvu_unittest_fmt, 128, "Expected: '%%.%is'\nGot:      '%%.%is'\n", (int)n, (int)n);\
        fprintf(stderr, "----------------\n");\
        fprintf(stderr, "Subtest failed at %s:%i.\n", __FILE__, __LINE__);\
        fprintf(stderr, tvu_unittest_fmt, s2, s1);\
        fprintf(stderr, "----------------\n");\
        return 1;\
    }

#define TVU_UNITTEST_STRCMP(s1, s2)\
    if (strcmp(s1, s2) != 0) {\
        fprintf(stderr, "----------------\n");\
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




#endif

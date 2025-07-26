#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "test.h"

#define RAND_TESTS 4096
#define RAND_COUNTS 64
#define RAND_EPSILON (RAND_TESTS / RAND_COUNTS / 2)
#define MIN_RAND_COUNT ((RAND_TESTS / RAND_COUNTS) - RAND_EPSILON)
#define MAX_RAND_COUNT ((RAND_TESTS / RAND_COUNTS) + RAND_EPSILON)

#define ALLOC_COUNT 4

#define SORT_SIZE 16
#define MAX_STR_LEN 8

static void test_rand(void)
{
    int i;
    int counts[RAND_COUNTS];

    begin("rand");

    memset(counts, 0, sizeof(counts));

    for (i = 0; i < RAND_TESTS; i++)
    {
        int r = rand();

        // Returns a pseudo-random integer value between ​0​ and RAND_MAX (0 and RAND_MAX included)
        if (r < 0)
        {
            failure();
            printf(" (%d<0)", r);
            return;
        }

        // Returns a pseudo-random integer value between ​0​ and RAND_MAX (0 and RAND_MAX included)
        if (r > RAND_MAX)
        {
            failure();
            printf(" (%d>RAND_MAX)", r);
            return;
        }

        counts[r % RAND_COUNTS]++;
    }

    success();

    for (i = 0; i < RAND_COUNTS; i++)
    {
        if (counts[i] < MIN_RAND_COUNT || counts[i] > MAX_RAND_COUNT)
        {
            failure();
            printf(" (count[%d]=%d)", i, counts[i]);
            return;
        }
    }

    success();
}

static void test_srand(void)
{
    int i;
    int seq[RAND_COUNTS];

    begin("srand");

    srand(1);

    for (i = 0; i < RAND_COUNTS; i++)
    {
        seq[i] = rand();
    }

    srand(1);

    for (i = 0; i < RAND_COUNTS; i++)
    {
        int r = rand();
        if (seq[i] != r)
        {
            failure();
            printf(" (seq[%d]: %d!=%d)", i, seq[i], r);
            return;
        }
    }

    success();
}

static void test_atof(void)
{
    int r = randInt(1000, 9999);
    char str[20];

    begin("atof");

#if !defined(CC65) && !defined(LLVM)
    sprintf(str, "0.%d", r);

    if (!expect(abs((int)(atof(str) * 10000.0) - r) <= 1))
    {
        printf(" (%s!=%.4f)", str, atof(str));
    }

    sprintf(str, "-0.%d", r);

    if (!expect(abs((int)(atof(str) * 10000.0) - -r) <= 1))
    {
        printf(" (%s!=%.4f)", str, atof(str));
    }
#else
    missing();
#endif
}

static void test_atoi(void)
{
    int r = randInt(1000, 9999);
    char str[20];

    begin("atoi");

    sprintf(str, "%d", r);

    if (!expect(atoi(str) == r))
    {
        printf(" (%s!=%d)\n", str, r);
    }

    sprintf(str, "-%d", r);

    if (!expect(atoi(str) == -r))
    {
        printf(" (%s!=%d)\n", str, -r);
    }
}

static void test_atol(void)
{
    long r = randLong(1000, 9999);
    char str[20];

    begin("atol");

    // It's a bit tricky to test atol when LONG support in Oscar64 is disabled.
    sprintf(str, "%d000", r);

    if (!expect(atol(str) == r * 1000L))
    {
        printf(" (%s!=%d000)\n", str, r * 1000L);
    }

    sprintf(str, "-%d000", r);

    if (!expect(atol(str) == -r * 1000L))
    {
        printf(" (%s!=%d000)\n", str, -r * 1000L);
    }
}

void test_malloc()
{
    int i, j;
    int sizes[ALLOC_COUNT];
    char str[ALLOC_COUNT][MAX_STR_LEN];
    void *ptr[ALLOC_COUNT];

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        sizes[i] = randInt(MAX_STR_LEN / 2, MAX_STR_LEN);
        randCharStr(str[i], sizes[i]);
    }

    begin("malloc");

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        ptr[i] = malloc(sizes[i]);

        if (ptr[i] == NULL)
        {
            failure();

            for (j = 0; j < i; j++)
            {
                free(ptr[j]);
            }

            return;
        }

        strcpy(ptr[i], str[i]);
    }

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        if (strcmp(ptr[i], str[i]) != 0)
        {
            failure();

            for (j = 0; j < ALLOC_COUNT; j++)
            {
                free(ptr[j]);
            }

            return;
        }
    }

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        free(ptr[i]);
    }

    success();
}

static void test_calloc(void)
{
    int i, j;
    int sizes[ALLOC_COUNT];
    char str[ALLOC_COUNT][MAX_STR_LEN];
    void *ptr[ALLOC_COUNT];

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        sizes[i] = randInt(MAX_STR_LEN / 4, MAX_STR_LEN / 2) * 2;
        randCharStr(str[i], sizes[i]);
    }

    begin("calloc");

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        ptr[i] = calloc(sizes[i] / 2, 2);

        if (ptr[i] == NULL)
        {
            failure();

            for (j = 0; j < i; j++)
            {
                free(ptr[j]);
            }

            return;
        }

        strcpy(ptr[i], str[i]);
    }

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        if (strcmp(ptr[i], str[i]) != 0)
        {
            failure();

            for (j = 0; j < ALLOC_COUNT; j++)
            {
                free(ptr[j]);
            }

            return;
        }
    }

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        free(ptr[i]);
    }

    success();
}

static void test_realloc(void)
{
    int i, j;
    int sizes[ALLOC_COUNT];
    char str[ALLOC_COUNT][MAX_STR_LEN];
    void *ptr[ALLOC_COUNT];

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        sizes[i] = randInt(MAX_STR_LEN / 2, MAX_STR_LEN);
        randCharStr(str[i], sizes[i]);
    }

    begin("realloc");

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        ptr[i] = malloc(MAX_STR_LEN / 2);

        if (ptr[i] == NULL)
        {
            failure();

            for (j = 0; j < i; j++)
            {
                free(ptr[j]);
            }

            return;
        }

        memcpy(ptr[i], str[i], MAX_STR_LEN / 2);
    }

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        ptr[i] = realloc(ptr[i], sizes[i]);

        if (memcmp(ptr[i], str[i], MAX_STR_LEN / 2) != 0)
        {
            failure();

            for (j = 0; j < ALLOC_COUNT; j++)
            {
                free(ptr[j]);
            }

            return;
        }

        strcpy(ptr[i], str[i]);
    }

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        if (strcmp(ptr[i], str[i]) != 0)
        {
            failure();

            for (j = 0; j < ALLOC_COUNT; j++)
            {
                free(ptr[j]);
            }

            return;
        }
    }

    for (i = 0; i < ALLOC_COUNT; i++)
    {
        free(ptr[i]);
    }

    success();
}

void test_qsort(void)
{
    int i;
    char arr[SORT_SIZE][MAX_STR_LEN];

    begin("qsort");

#if !defined(OSCAR64)
    for (i = 0; i < SORT_SIZE; i++)
    {
        randCharStr(arr[i], randInt(1, MAX_STR_LEN));
    }

    qsort(arr, SORT_SIZE, MAX_STR_LEN, (int (*)(const void *, const void *))strcmp);

    for (i = 1; i < SORT_SIZE; i++)
    {
        if (strcmp(arr[i - 1], arr[i]) > 0)
        {
            failure();
            printf(" (%s > %s)", arr[i - 1], arr[i]);
            return;
        }
    }

    success();
#else
    missing();
#endif
}

static void test_abs(void)
{
    int r = randInt(1000, 9999);
    int negR = -r;

    begin("abs");

    expect(abs(r) == r);
    expect(abs(negR) == r);
}

static void test_labs(void)
{
    long r = randLong(1000000, 9999999);
    long negR = -r;

    begin("labs");

    expect(labs(r) == r);
    expect(labs(negR) == r);
}

static void test_div(void)
{
    int r = randInt(1000, 9999);
    int negR = -r;
    int d = randInt(1, 100);
    int negD = -d;

    div_t res;

    begin("div");

    res = div(r, d);
    expect(res.quot == r / d && res.rem == r % d);

    res = div(negR, d);
    expect(res.quot == negR / d && res.rem == negR % d);

    res = div(r, negD);
    expect(res.quot == r / negD && res.rem == r % negD);

    res = div(negR, negD);
    expect(res.quot == negR / negD && res.rem == negR % negD);
}

static void test_ldiv(void)
{
    long r = randLong(1000000, 9999999);
    long negR = -r;
    long d = randLong(1, 100);
    long negD = -d;

    ldiv_t res;

    begin("ldiv");

#if !defined(CC65)
    res = ldiv(r, d);
    expect(res.quot == r / d && res.rem == r % d);

    res = ldiv(negR, d);
    expect(res.quot == negR / d && res.rem == negR % d);

    res = ldiv(r, negD);
    expect(res.quot == r / negD && res.rem == r % negD);

    res = ldiv(negR, negD);
    expect(res.quot == negR / negD && res.rem == negR % negD);
#else
    missing();
#endif
}

void test(void)
{
    test_rand();
    test_srand();

    test_atof();
    test_atoi();
    test_atol();

    test_malloc();
    test_calloc();
    test_realloc();

    test_qsort();

    test_abs();
    test_labs();

    test_div();
    test_ldiv();
}

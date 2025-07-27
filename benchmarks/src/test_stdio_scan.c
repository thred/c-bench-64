#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

#define BUFFER_SIZE 32

void test_scanf_c(void)
{
    char c;
    char str[BUFFER_SIZE];

    begin("scanf-c");

#if !defined(SDCC)
    sscanf("AB DE", "%c", &c);

    if (!expect(c == 'A'))
    {
        printf(" (%c)", c);
    }

    memset(str, '#', BUFFER_SIZE);
    sscanf("AB DE", "%3c", str);

    if (!expect(memcmp(str, "AB #", 4) == 0))
    {
        str[4] = '\0';
        printf(" (%s)", str);
    }
#else
    missing();
#endif
}

void test_scanf_s(void)
{
    char str[BUFFER_SIZE];

    begin("scanf-s");

#if !defined(SDCC)
    sscanf("Hello, World!", "%s", str);

    if (!expect(strcmp(str, "Hello,") == 0))
    {
        printf(" (%s)", str);
    }

    sscanf("Hello, World!", "%3s", str);

    if (!expect(strcmp(str, "Hel") == 0))
    {
        printf(" (%s)", str);
    }
#else
    missing();
#endif
}

void test_scanf_d(void)
{
    int value;

    begin("scanf-d");

#if !defined(SDCC)
    sscanf("42", "%d", &value);

    if (!expect(value == 42))
    {
        printf(" (%d)", value);
    }
#else
    missing();
#endif
}

void test_scanf_i(void)
{
    int value;

    begin("scanf-i");

#if !defined(SDCC)
    sscanf("-42", "%i", &value);

    if (!expect(value == -42))
    {
        printf(" (%d)", value);
    }
#else
    missing();
#endif
}

void test_scanf_u(void)
{
    unsigned int value;

    begin("scanf-u");

#if !defined(SDCC)
    sscanf("65535", "%u", &value);

    if (!expect(value == 65535))
    {
        printf(" (%u)", value);
    }
#else
    missing();
#endif
}

void test_scanf_x(void)
{
    unsigned int value;

    begin("scanf-x");

#if !defined(SDCC)
    sscanf("cafe", "%x", &value);

    if (!expect(value == 0xcafe))
    {
        printf(" (%x)", value);
    }
#else
    missing();
#endif
}

void test_scanf_X(void)
{
    unsigned int value;

    begin("scanf-X");

#if !defined(SDCC)
    sscanf("CAFE", "%X", &value);

    if (!expect(value == 0xcafe))
    {
        printf(" (%x)", value);
    }
#else
    missing();
#endif
}

void test_scanf_f(void)
{
#if !defined(__FLOAT_MISSING) && !defined(LLVM) && !defined(SDCC)
    float value;

    begin("scanf-f");

#if !defined(NOFLOAT)
    sscanf("3.14", "%f", &value);

    if (!expect(closeTo(value, 3.14)))
    {
        printf(" (%f)", value);
    }
#else
    disabled();
#endif
#else
    begin("scanf-f");
    missing();
#endif
}

void test_scanf_e(void)
{
#if !defined(__FLOAT_MISSING) && !defined(LLVM) && !defined(SDCC)
    float value;

    begin("scanf-e");

#if !defined(NOFLOAT)
    sscanf("3.14e2", "%e", &value);

    if (!expect(closeTo(value, 314.0)))
    {
        printf(" (%f)", value);
    }
#else
    disabled();
#endif
#else
    begin("scanf-e");
    missing();
#endif
}

void test(void)
{
    test_scanf_c();
    test_scanf_s();
    test_scanf_d();
    test_scanf_i();
    test_scanf_u();
    test_scanf_x();
    test_scanf_X();
    test_scanf_f();
    test_scanf_e();
}

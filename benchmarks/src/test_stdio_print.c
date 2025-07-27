#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

#define BUFFER_SIZE 32

static void test_printf_c(void)
{
    char c = 'A';
    char buffer[BUFFER_SIZE];

    begin("printf-c");

    sprintf(buffer, "%c", c);

    if (!expect(strcmp(buffer, "A") == 0))
    {
        printf(" (%s!=A)", buffer);
    }
}

static void test_printf_s(void)
{
    const char *str = "Hello, World!";
    char buffer[BUFFER_SIZE];

    begin("printf-s");

    sprintf(buffer, "%s", str);

    if (!expect(strcmp(buffer, "Hello, World!") == 0))
    {
        printf(" (%s!=Hello, World!)", buffer);
    }
}

static void test_printf_d(void)
{
    int i = 42;
    char buffer[BUFFER_SIZE];

    begin("printf-d");

    sprintf(buffer, "%d", i);

    if (!expect(strcmp(buffer, "42") == 0))
    {
        printf(" (%s!=42)", buffer);
    }
}

static void test_printf_ld(void)
{
    long l = 1234567890L;
    char buffer[BUFFER_SIZE];

    begin("printf-ld");

#if !defined(NOLONG)
    sprintf(buffer, "%ld", l);

    if (!expect(strcmp(buffer, "1234567890") == 0))
    {
        printf(" (%s!=1234567890)", buffer);
    }
#else
    disabled();
#endif
}

static void test_printf_i(void)
{
    int i = -42;
    char buffer[BUFFER_SIZE];

    begin("printf-i");

    sprintf(buffer, "%i", i);

    if (!expect(strcmp(buffer, "-42") == 0))
    {
        printf(" (%s!=-42)", buffer);
    }
}

static void test_printf_li(void)
{
    long l = -1234567890L;
    char buffer[BUFFER_SIZE];

    begin("printf-li");

#if !defined(NOLONG)
    sprintf(buffer, "%li", l);

    if (!expect(strcmp(buffer, "-1234567890") == 0))
    {
        printf(" (%s!=-1234567890)", buffer);
    }
#else
    disabled();
#endif
}

static void test_printf_x(void)
{
    unsigned int hex = 0xcafe;
    char buffer[BUFFER_SIZE];

    begin("printf-x");

    sprintf(buffer, "%x", hex);

    if (!expect(strcmp(buffer, "cafe") == 0))
    {
        printf(" (%s!=cafe)", buffer);
    }
}

static void test_printf_lx(void)
{
    unsigned long hex = 0xdeadbeef;
    char buffer[BUFFER_SIZE];

    begin("printf-lx");

#if !defined(NOLONG)
    sprintf(buffer, "%lx", hex);

    if (!expect(strcmp(buffer, "deadbeef") == 0))
    {
        printf(" (%s!=deadbeef)", buffer);
    }
#else
    disabled();
#endif
}

static void test_printf_X(void)
{
    unsigned int hex = 0xcafe;
    char buffer[BUFFER_SIZE];

    begin("printf-X");

    sprintf(buffer, "%X", hex);

    if (!expect(strcmp(buffer, "CAFE") == 0))
    {
        printf(" (%s!=CAFE)", buffer);
    }
}

static void test_printf_lX(void)
{
    unsigned long hex = 0xdeadbeef;
    char buffer[BUFFER_SIZE];

    begin("printf-lX");

#if !defined(NOLONG)
    sprintf(buffer, "%lX", hex);

    if (!expect(strcmp(buffer, "DEADBEEF") == 0))
    {
        printf(" (%s!=DEADBEEF)", buffer);
    }
#else
    disabled();
#endif
}

static void test_printf_f(void)
{
#if !defined(__FLOAT_MISSING) && !defined(LLVM) && !defined(SDCC)
    float f = 3.14159;
    char buffer[BUFFER_SIZE];

    begin("printf-f");

#if !defined(NOFLOAT)
    sprintf(buffer, "%.2f", f);

    if (!expect(strcmp(buffer, "3.14") == 0))
    {
        printf(" (%s!=3.14)", buffer);
    }
#else
    disabled();
#endif
#else
    begin("printf-f");
    missing();
#endif
}

static void test_printf_e(void)
{
#if !defined(__FLOAT_MISSING) && !defined(LLVM) && !defined(SDCC)
    float d = 123456.789;
    char buffer[BUFFER_SIZE];

    begin("printf-e");

#if !defined(NOFLOAT)
    sprintf(buffer, "%.2e", d);

    if (!expect(strcmp(buffer, "1.23e+05") == 0))
    {
        printf(" (%s!=1.23e+05)", buffer);
    }
#else
    disabled();
#endif
#else
    begin("printf-e");
    missing();
#endif
}

void test(void)
{
    test_printf_c();
    test_printf_s();
    test_printf_d();
    test_printf_ld();
    test_printf_i();
    test_printf_li();
    test_printf_x();
    test_printf_lx();
    test_printf_X();
    test_printf_lX();
    test_printf_f();
    test_printf_e();
}

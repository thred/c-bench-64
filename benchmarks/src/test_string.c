#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "test.h"

// #define STRLEN 448
#define STRLEN 16

static void test_strlen(void)
{
    char str[STRLEN];
    size_t len = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("strlen");

    randCharStr((char *)str, len);

    expect(strlen(str) == len - 1);
}

static void test_strchr(void)
{
    char str[STRLEN];
    size_t n = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("strchr");

    randCharStr(str, STRLEN);
    str[n - 1] = '*';
    str[n + 1] = '*';

    expect(strchr(str, '*') == str + n - 1);
}

static void test_strrchr(void)
{
    char str[STRLEN];
    size_t n = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("strrchr");

    randCharStr(str, STRLEN);
    str[n - 1] = '*';
    str[n + 1] = '*';

    expect(strrchr(str, '*') == str + n + 1);
}

static void test_strcmp(void)
{
    char str[STRLEN], eqstr[STRLEN], neqstr[STRLEN];

    begin("strcmp");

    randCharStr(str, STRLEN);
    strcpy(eqstr, str);
    randCharStr(neqstr, STRLEN);

    expect(strcmp(str, eqstr) == 0);
    expect(strcmp(str, neqstr) != 0);
}

static void test_strncmp(void)
{
    char str[STRLEN], eqstr[STRLEN];
    size_t len = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("strncmp");

    randCharStr(str, STRLEN);
    strncpy(eqstr, str, len);
    (eqstr)[len] = '\0';

    expect(strncmp(str, eqstr, len - 1) == 0);
    expect(strncmp(str, eqstr, len + 1) != 0);
}

static void test_strcpy(void)
{
    char src[STRLEN], dest[STRLEN];

    begin("strcpy");

    randCharStr(src, STRLEN);

    strcpy(dest, src);

    expect(strcmp(dest, src) == 0);
}

static void test_strncpy(void)
{
    char src[STRLEN], dest[STRLEN];
    size_t len = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("strncpy");

    randCharStr(src, STRLEN);

    strncpy(dest, src, len);
    dest[len] = '\0';

    expect(strncmp(dest, src, len) == 0);
}

static void test_strcat(void)
{
    char src[STRLEN], dest[STRLEN], suffix[STRLEN];
    size_t len = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("strcat");

    randCharStr(src, STRLEN);

    strncpy(dest, src, len);
    dest[len] = '\0';
    strncpy(suffix, src + len, STRLEN - len);

    strcat(dest, suffix);

    expect(strcmp(src, dest) == 0);
}

static void test_strncat(void)
{
    char src[STRLEN], dest[STRLEN], suffix[STRLEN];
    size_t len = randInt(1, STRLEN / 4);
    size_t suffixLen = randInt(STRLEN / 4, STRLEN / 2 - 1);

    begin("strncat");

    randCharStr(src, STRLEN);

    strncpy(dest, src, len);
    dest[len] = '\0';
    strncpy(suffix, src + len, STRLEN - len);

    strncat(dest, suffix, suffixLen);

    expect(strncmp(src, dest, len + suffixLen) == 0);
    expect(dest[len + suffixLen] == '\0');
}

static void test_strstr(void)
{
    char str[STRLEN];
    size_t n = randInt(1, STRLEN - 6);

    begin("strstr");

    randCharStr(str, STRLEN);
    memcpy(str + n, "*-*-*", 5);

    expect(strstr(str, "*-*-*") == str + n);
}

static void test_memchr(void)
{
    char str[STRLEN];
    size_t n = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("memchr");

    randCharStr(str, STRLEN);
    str[n - 1] = '*';

    expect(memchr(str, '*', STRLEN) == str + n - 1);
}

static void test_memset(void)
{
    int i;
    char str[STRLEN];
    size_t n = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("memset");

    randCharStr(str, STRLEN);
    memset(str, '*', n);

    for (i = 0; i < STRLEN; i++)
    {
        if ((i < n && str[i] != '*') || (i >= n && str[i] == '*'))
        {
            failure();
            return;
        }
    }

    success();
}

static void test_memcpy(void)
{
    int i;
    char src[STRLEN], dest[STRLEN];
    size_t n = randInt(STRLEN * 3 / 4, STRLEN - 1);

    begin("memcpy");

    randCharStr(src, STRLEN);
    randDigitStr(dest, STRLEN);

    memcpy(dest, src, n);

    for (i = 0; i < n; i++)
    {
        if ((i < n && dest[i] != src[i]) || (i >= n && dest[i] == src[i]))
        {
            failure();
            return;
        }
    }

    success();
}

static void test_memmove(void)
{
    char str[STRLEN], cmp[STRLEN];
    size_t n = randInt(STRLEN / 4, STRLEN / 2 - 1);
    size_t len = STRLEN / 2 - 1;

    begin("memmove");

    randCharStr(str, STRLEN);
    memcpy(cmp, str, STRLEN); // Save original for comparison

    // move forward
    memmove(str + n, str, len);

    expect(memcmp(str, cmp, len - n) == 0);
    expect(memcmp(str + n, cmp, len) == 0);
    expect(memcmp(str + n + len, cmp + n + len, STRLEN - n - len) == 0);

    randCharStr(str, STRLEN);
    memcpy(cmp, str, STRLEN); // Save original for comparison

    // move backward
    memmove(str, str + n, len);

    expect(memcmp(str, cmp + n, len) == 0);
    expect(memcmp(str + len, cmp + len, STRLEN - len) == 0);
}

void test(void)
{
    test_strlen();
    test_strchr();
    test_strrchr();
    test_strcmp();
    test_strncmp();
    test_strcpy();
    test_strncpy();
    test_strcat();
    test_strncat();
    test_strstr();

    test_memchr();
    test_memset();
    test_memcpy();
    test_memmove();
}

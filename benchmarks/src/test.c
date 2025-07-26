#include "test.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if !defined(CC65)
#include <math.h>
#endif

#include "cia_timer.h"

#define EPSILON 0.001

void test(void);

#ifdef KICKC
#define MAIN void main()
#else
#define MAIN int main()
#endif

#if defined(LLVM) || defined(VBCC)
#define VICETRAP 0xc000
static volatile unsigned char *viceTrapPtr = (unsigned char *)VICETRAP;
static void (*viceTrap)(void) = (void (*)(void))VICETRAP;
#endif

MAIN
{
    unsigned int t;

#ifdef OSCAR64
    *(unsigned char *)0xd018 = 0x17; // Set VIC-II to lower case mode
#endif

    tod_init(0);

    test();

    t = tod_get10();

    // sometimes, the access to t causes a seg fault with OSCAR64, e.g. with test-math
#ifndef OSCAR64
    printf("\nTotal time: %01d.%01d s\n", t / 10, t % 10);
#endif

#if defined(LLVM) || defined(VBCC)
    fflush(stdout);
#endif

    // Wait a bit to ensure a refresh before taking a screenshot
    tod_init(0);

#if defined(LLVM) || defined(VBCC)
    // Trigger the VICE monitor to exit
    *viceTrapPtr = 0x60; // RTS opcode
    viceTrap();
#endif

#ifndef KICKC
    return 0;
#endif
}

void begin(const char *test)
{
    printf("\n#%s", test);
}

void success(void)
{
    printf(" [OK]");
}

void failure(void)
{
    printf(" [FAIL]");
}

void missing()
{
    printf(" [MISS]");
}

void disabled()
{
    printf(" [OFF]");
}

char expect(char result)
{
    if (result)
    {
        success();
    }
    else
    {
        failure();
    }

    return result;
}

#if !defined(CC65)
char closeTo(float a, float b)
{
    float diff;

    while (a < -1.1 || a > 1.1)
        a /= 10.0;

    while (b < -1.1 || b > 1.1)
        b /= 10.0;

    diff = a - b;

    return diff >= -EPSILON && diff <= EPSILON;
}

char expectCloseTo(float a, float b)
{
    char result = expect(closeTo(a, b));

    if (!result)
    {
        printf(" (%f != %f)", a, b);
    }

    return result;
}
#endif

void randStr(const char *charset, char *str, size_t len)
{
    size_t charsetLen = strlen(charset);
    size_t i;

    assert(len > 0);

    for (i = 0; i < len - 1; i++)
    {
        // some implementations of rand() return negative values
        str[i] = charset[abs(rand()) % charsetLen];
    }

    str[len - 1] = '\0';
}

void randCharStr(char *str, size_t len)
{
    randStr("abcdefghijklmnopqrstuvwxyz", str, len);
}

void randDigitStr(char *str, size_t len)
{
    randStr("0123456789", str, len);
}

int randInt(int min, int max)
{
    // some implementations of rand() return negative values
    return min + (abs(rand()) % (max - min));
}

long randLong(long min, long max)
{
    // some implementations of rand() return negative values
    unsigned long value = ((unsigned long)(abs(rand()) & 0xFFFF) << 12) ^
                          ((unsigned long)(abs(rand()) & 0xFFFF) << 5) ^
                          (unsigned long)(abs(rand()) & 0xFFFF);

    return min + (value % (max - min));
}

unsigned long crc32(const unsigned char *data, unsigned int length)
{
    unsigned char extract;
    unsigned int i;
    unsigned char j;
    unsigned long crc;

    crc = 0;

    for (i = 0; i < length; i++)
    {
        extract = *data;
        crc ^= (((unsigned long)extract) << 24);

        for (j = 8; j; j--)
        {
            if ((crc & 0x80000000) != 0)
            {
                crc <<= 1;
                crc ^= 0x04c11db7;
            }
            else
                crc <<= 1;
        }
        data++;
    }

    crc ^= 0xffffffff;

    return crc;
}

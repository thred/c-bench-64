#include <stdio.h>

#include "out.h"

void print(const char *s)
{
    while (*s)
    {
        char c = *s++;

#ifdef CALYPSI
        if (c >= 'A' && c <= 'Z') {
            c += 0x80; // Convert ASCII uppercase to PETSCII uppercase (lowercase in C64 mode)
        } else if (c >= 'a' && c <= 'z') {
            c -= 0x20; // Convert ASCII lowercase to PETSCII lowercase (uppercase in C64 mode)
        } else if (c == '\n') {
            c = 13; // Convert ASCII newline to PETSCII newline (CR)
        }
#endif

        putchar(c);
    }
}

void print_int(long n, int padLength)
{
    char buffer[12]; // Enough for int32_t
    int i = 0;
    int isNegative = 0;

    if (n < 0)
    {
        isNegative = 1;
        n = -n;
    }

    if (n == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while (n > 0)
        {
            buffer[i++] = '0' + (n % 10);
            n /= 10;
        }
    }

    // Pad with zeros if needed
    while (i < padLength)
    {
        buffer[i++] = '0';
    }

    if (isNegative)
    {
        putchar('-');
    }

    while (i > 0)
    {
        putchar(buffer[--i]);
    }
}

void print_hex(unsigned long n, int padLength)
{
    char buffer[9]; // Enough for uint32_t in hex
    int i = 0;

    if (n == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while (n > 0)
        {
            unsigned int digit = n & 0xF;
            buffer[i++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
            n >>= 4;
        }
    }

    // Pad with zeros if needed
    while (i < padLength)
    {
        buffer[i++] = '0';
    }

    while (i > 0)
    {
        putchar(buffer[--i]);
    }
}
#include "cia_timer.h"
#include "out.h"

#ifndef SDCC
#define __reentrant
#endif

#define N_ITER 1000

#define SIZE 16

long array[SIZE];

long fact(long i) __reentrant // Change parameter and return type
{
    if (i > 1)
        return i * fact(i - 1);
    else
        return 1;
}

long res;

void benchmark(void)
{
    int i, j;

    res = 0;

    for (j = 0; j < SIZE; j++)
    {
        array[j] = 0;
    }

    for (i = 0; i < N_ITER; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            array[j] += fact(j);
        }
    }

    for (j = 0; j < SIZE; j++)
    {
        res += array[j];
    }
}

void benchmark_name(void)
{
    print("fact.c\n");
    print("Calculates factorials (");
    print_int(N_ITER, 0);
    print(" iterations)\n");
}

const long expected = 188806544;

unsigned char benchmark_check(void)
{
    print("res=");
    print_int(res, 0);

    if (res == expected)
    {
        print(" [OK]");
        return 0;
    }

    print(" [FAIL] - expected ");
    print_int(expected, 0);
    return 1;
}

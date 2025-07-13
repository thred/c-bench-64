#include <stdio.h>
#include "cia_timer.h"

#define N_ITER 10

#define SIZE 32

#define FPTYPE float
// #define FPTYPE double
// #define FPTYPE long double // use %Lf for printing

FPTYPE array[SIZE];

FPTYPE testpow(FPTYPE x, unsigned int y)
{
    unsigned char i;
    FPTYPE tmp = x;

    if (y == 0)
        return 1;

    for (i = 1; i < y; i++)
        tmp *= x;

    return tmp;
}

FPTYPE res;

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
            array[j] += testpow(2.5 / (i + 1), j);
        }
    }

    for (j = 0; j < SIZE; j++)
    {
        res += array[j];
    }
}

void benchmark_name(void)
{
    printf("pow.c\n");
    printf("Calculates floating point exponential (%d iterations)\n", N_ITER);
}

const FPTYPE expected = 3614007361536.000000;
const FPTYPE epsilon = 10000000;

unsigned char benchmark_check(void)
{
    printf("res      = %f ", res);

    FPTYPE diff = expected - res;

    printf("\nexpected = %f ", expected);
    printf("\nepsilon  = %f ", epsilon);
    printf("\ndiff     = %f ", diff);

    if (diff < epsilon && diff > (long)-epsilon)
    {
        printf("[OK]\n");
        return 0;
    }

    printf("[FAIL]\n");
    return 1;
}

// compile it with: gcc pow.c -D VERIFY -o pow
#ifdef VERIFY
int main(void)
{
    benchmark_name();
    benchmark();

    return benchmark_check();
}
#endif
#include "cia_timer.h"
#include "out.h"

#define N_ITER 10

#define SIZE 8191
#define EXPECTED 1900

char flags[SIZE];

unsigned int prime_count;

unsigned int sieve(unsigned int n)
{
    unsigned int i, prime, k, count;

    unsigned int size = n;

    count = 1;

    for (i = 0; i < size; i++)
        flags[i] = 1;

    for (i = 0; i < size; i++)
    {
        if (flags[i])
        {
#ifdef KICKC
            prime = i * 2 + 3;
#else
            prime = i + i + 3;
#endif
            k = i + prime;
            while (k < size)
            {
                flags[k] = 0;
                k += prime;
            }
            count++;
        }
    }

    return count;
}

void benchmark(void)
{
    int i;
    for (i = 0; i < N_ITER; i++)
        prime_count = sieve(SIZE);
}

void benchmark_name(void)
{
    print("sieve.c\n");
    print("Calculates the primes from 1 to ");
    print_int(SIZE * 2 + 2, 0);
    print(" (");
    print_int(N_ITER, 0);
    print(" iterations)\n");
}

unsigned char benchmark_check(void)
{
    print("count=");
    print_int(prime_count, 0);

    if (prime_count == EXPECTED)
    {
        print(" [OK]");
        return 0;
    }

    print(" [FAIL] - expected");
    print_int(EXPECTED, 0);
    return 1;
}

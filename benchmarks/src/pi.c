#include "cia_timer.h"
#include "out.h"

#define SCALE 10000
#define ARRINIT 2000

#define NUM_DIG 560

long carry = 0;
long arr[NUM_DIG + 1];

void pi_digits(int digits)
{
    int i, j;
    long sum;

    for (i = 0; i <= digits; ++i)
        arr[i] = ARRINIT;
    for (i = digits; i > 0; i -= 14)
    {
        sum = 0;
        for (j = i; j > 0; --j)
        {
            sum = sum * j + SCALE * arr[j];
            arr[j] = sum % (j * 2 - 1);
            sum /= j * 2 - 1;
        }
        print_int(carry + sum / SCALE, 4);
        carry = sum % SCALE;
    }
}

void benchmark(void)
{
    pi_digits(NUM_DIG);
}

void benchmark_name(void)
{
    print("pi.c\n");
    print("Calculates ");
    print_int(4 * NUM_DIG / 14, 0);
    print(" digits of pi\n");
}

#define EXPECTED 2822

unsigned char benchmark_check(void)
{
    print("carry=");
    print_int(carry, 0);

    if (carry == EXPECTED)
    {
        print(" [OK]");
        return 0;
    }

    print(" [FAIL] - expected ");
    print_int(EXPECTED, 0);
    return 1;
}

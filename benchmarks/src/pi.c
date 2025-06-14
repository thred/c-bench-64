#include <stdio.h>
#include "cia_timer.h"

#define SCALE 10000
#define ARRINIT 2000

#define NUM_DIG 560

long arr[NUM_DIG + 1];

void pi_digits(int digits) {
    long carry = 0;
    int i,j;
    long sum;

    for (i = 0; i <= digits; ++i)
        arr[i] = ARRINIT;
    for (i = digits; i > 0; i-= 14) {
        sum = 0;
        for (j = i; j > 0; --j) {
            sum = sum * j + SCALE * arr[j];
            arr[j] = sum % (j * 2 - 1);
            sum /= j * 2 - 1;
        }
        printf("%04ld", carry + sum / SCALE);
        carry = sum % SCALE;
    }
}

void benchmark(void)
{
  pi_digits(NUM_DIG);
}

void benchmark_name(void)
{
  printf("pi.c\n");
  printf("Calculates %d digits of pi\n",4*NUM_DIG/14);
}

unsigned char benchmark_check(void)
{
  return 0;
}

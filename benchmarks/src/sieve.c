#include <stdio.h>
#include "cia_timer.h"

#define N_ITER 10

#define SIZE 8191
#define EXPECTED 1900

char flags[SIZE];

unsigned int prime_count;

unsigned int sieve(unsigned int n)
{
  unsigned int i, prime, k, count;

  unsigned int size=n;

    count=1;

    for (i = 0; i < size; i++)
      flags[i] = 1;

    for (i = 0; i < size; i++) {
      if (flags[i]) {
#ifdef KICKC
	prime = i*2 + 3;
#else
	prime = i + i + 3;
#endif
	k = i + prime;
	while (k < size) {
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
  for(i=0;i<N_ITER;i++)
    prime_count=sieve(SIZE);
}

void benchmark_name(void)
{
  printf("sieve.c\n");
  printf("Calculates the primes from 1 to %d (%d iterations)\n",SIZE*2+2,N_ITER);
}

unsigned char benchmark_check(void)
{
  printf("count=%u ",prime_count);
  if(prime_count==EXPECTED) {
    printf("OK\n");
    return 0;
  } else {
    printf("FAIL - expected %d\n",EXPECTED);
    return 1;
  }
}

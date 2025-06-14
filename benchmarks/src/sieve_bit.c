#include <stdio.h>
#include "cia_timer.h"

#define N_ITER 4

#define SIZE 16000
#define EXPECTED 3432

unsigned char flags[(SIZE/8)+1];

unsigned int prime_count;

const unsigned char bitv[] = {
  0x01, 0x02, 0x04, 0x08,
  0x10, 0x20, 0x40, 0x80,
};
  

unsigned char check_flag(unsigned int idx)
{
  return flags[idx/8]&bitv[idx&7];
  //  return flags[idx/8]&(0x1<<(idx&7));
}

void clear_flag(unsigned int idx)
{
  flags[idx/8]&= ~((unsigned char)bitv[idx&7]);
  //  flags[idx/8]&= ~((unsigned char)(0x1<<(idx&7)));
}

unsigned int sieve(unsigned int n)
{
  unsigned int i, prime, k, count;

  unsigned int size=n;

    count=1;

    for (i = 0; i < (size/8)+1 ; i++)
      flags[i] = 0xff;

    for (i = 0; i < size; i++) {
      if (check_flag(i)) {
#ifdef KICKC
	prime = i*2 + 3;
#else
	prime = i + i + 3;
#endif
	k = i + prime;
	while (k < size) {
	  clear_flag(k);
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
  printf("sieve_bit.c\n");
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

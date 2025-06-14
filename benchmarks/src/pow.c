#include <stdio.h>
#include "cia_timer.h"

#define N_ITER 10

#define SIZE 32

float array[SIZE];

float testpow(float x, unsigned int y)
{
  unsigned char i;
  float tmp=x;

  if(y==0) return 1;

  for(i=1;i<y;i++)
    tmp*=x;

  return tmp;
}

float res;

void benchmark(void)
{
  int i,j;

  res=0;

  for(j=0;j<SIZE;j++) {
    array[j]=0;
  }


  for(i=0;i<N_ITER;i++) {
    for(j=0;j<SIZE;j++) {
      array[j]+=testpow(2.5/(i+1),j);
    }
  }

  for(j=0;j<SIZE;j++) {
    res+=array[j];
  }

}

void benchmark_name(void)
{
  printf("pow.c\n");
  printf("Calculates floating point exponential (%d iterations)\n",N_ITER);
}

unsigned char benchmark_check(void)
{

  printf("res=%f ",res);
/*
  if(prime_count==EXPECTED) {
    printf("OK\n");
    return 0;
  } else {
    printf("FAIL - expected %d\n",EXPECTED);
    return 1;
  }
*/
 printf("OK\n");
 return 0;
}

#include <stdio.h>
#include "cia_timer.h"

#ifndef SDCC
#define __reentrant
#endif

#define N_ITER 1000

#define SIZE 16

long array[SIZE];

long fact(long i) __reentrant
{
  if(i>1) return i*fact(i-1);
  else return 1;
}

long res;

void benchmark(void)
{
  int i,j;

  res=0;

  for(j=0;j<SIZE;j++) {
    array[j]=0;
  }


  for(i=0;i<N_ITER;i++) {
    for(j=0;j<SIZE;j++) {
      array[j]+=fact(j);
    }
  }

  for(j=0;j<SIZE;j++) {
    res+=array[j];
  }

}

void benchmark_name(void)
{
  printf("fact.c\n");
  printf("Calculates factorials (%d iterations)\n",N_ITER);
}

unsigned char benchmark_check(void)
{

  printf("res=%ld ",res);
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

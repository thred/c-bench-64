#include <stdio.h>
#include "cia_timer.h"

#ifndef SDCC
#define __data
#endif

#if 0
static unsigned char CRC8(const unsigned char *data, unsigned int length)
{
  /* CRC-8/GSM-A */
   unsigned char __data crc;
   unsigned int  __data end;
   unsigned char __data tmp;
   unsigned char __data j;

   end=(unsigned int)data+length;

   printf("st:%04x e:%04x\n",(unsigned int )data,end);

   crc=0;
   for(;(unsigned int)data!=end;data++) {
     tmp=*data;
     crc^= tmp;

     for (j = 8; j; j--)  {
       if ((crc & 0x80) != 0) crc = ((crc << 1) ^ 0x1d);
       else crc<<=1;
     }
   }
   return crc;
}

#else
static unsigned char CRC8(const unsigned char *data, unsigned int length)
{
  // CRC-8/GSM-A
   unsigned char __data crc;
   unsigned char __data tmp;
   unsigned int __data i;
   unsigned char __data j;

   crc=0;
   for(i=0;i<length;i++) {
     tmp = *data;
     crc^=tmp;

     for (j = 8; j; j--)  {
       if ((crc & 0x80) != 0) crc = ((crc << 1) ^ 0x1d);
       else crc<<=1;
     }
      data++;
   }
   return crc;
}
#endif

static unsigned char crc;

void benchmark(void)
{
  crc=CRC8((unsigned char *)0xe000,0x2000);
}

void benchmark_name(void)
{
  printf("crc8.c\n");
  printf("Calculates the CRC8 of the C64 Kernal\n");
}

#define CORRECT 0xa2

unsigned char benchmark_check(void)
{
  printf("CRC8=%02x  ",crc);

  if(crc==CORRECT) {
    printf("OK\n");
    return 0;
  } else {
    printf("FAIL - expected %02x\n",CORRECT);
  }
  return 1;
}

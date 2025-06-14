#include <stdio.h>

unsigned int CRC16(const unsigned char *data, unsigned int length) 
{
  /* CRC-16/XMODEM */
   unsigned int crc;
   unsigned char extract;
   unsigned int i;
   unsigned char j;

   crc=0;
   for(i=0;i<length;i++) {
     extract=*data;
#ifdef KICKC
     >crc = >crc ^ extract;
#else
     crc^=(extract<<8);
#endif

     for (j = 8; j; j--) {
       if((crc&0x8000)!=0) crc=(crc<<1)^0x1021;
       else crc<<=1;
       }
     data++;
   }
   return crc;
}

static unsigned int crc;


void benchmark(void)
{
  crc=CRC16((unsigned char *)0xe000,0x2000);
}

void benchmark_name(void)
{
  printf("crc16.c\n");
  printf("Calculates the CRC16 of the C64 Kernal\n");
}

#define CORRECT 0xffd0

unsigned char benchmark_check(void)
{
  printf("CRC16=%04x  ",crc);

  if(crc==CORRECT) {
    printf("OK\n");
    return 0;
  } else {
    printf("FAIL - expected %04x\n",CORRECT);
  }
  return 1;
}


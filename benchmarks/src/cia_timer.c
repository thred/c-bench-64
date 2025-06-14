#include "cia_timer.h"

#define CNT 200

struct __6526 {
    unsigned char       pra;            /* Port register A */
    unsigned char       prb;            /* Port register B */
    unsigned char       ddra;           /* Data direction register A */
    unsigned char       ddrb;           /* Data direction register B */
    unsigned char       ta_lo;          /* Timer A, low byte */
    unsigned char       ta_hi;          /* Timer A, high byte */
    unsigned char       tb_lo;          /* Timer B, low byte */
    unsigned char       tb_hi;          /* Timer B, high byte */
    unsigned char       tod_10;         /* TOD, 1/10 sec. */
    unsigned char       tod_sec;        /* TOD, seconds */
    unsigned char       tod_min;        /* TOD, minutes */
    unsigned char       tod_hour;       /* TOD, hours */
    unsigned char       sdr;            /* Serial data register */
    unsigned char       icr;            /* Interrupt control register */
    unsigned char       cra;            /* Control register A */
    unsigned char       crb;            /* Control register B */
};

#ifdef KICKC
#define CIA2    (*((/*volatile*/ struct __6526*)0xDD00))
#else
#define CIA2    (*((volatile struct __6526*)0xDD00))
#endif

#define CIA2_TA_LO    (*(unsigned char *)0xdd04)
#define CIA2_TA_HO    (*(volatile unsigned char *)0xdd05)
#define CIA2_TB_LO    (*(volatile unsigned char *)0xdd06)
#define CIA2_TB_HO    (*(volatile unsigned char *)0xdd07)
#define CIA2_TOD_10   (*(volatile unsigned char *)0xdd08)
#define CIA2_TOD_SEC  (*(volatile unsigned char *)0xdd09)
#define CIA2_TOD_MIN  (*(volatile unsigned char *)0xdd0a)
#define CIA2_TOD_HOUR (*(volatile unsigned char *)0xdd0b)
#define CIA2_CRA      (*(volatile unsigned char *)0xdd0e)
#define CIA2_CRAB     (*(volatile unsigned char *)0xdd0f)



static unsigned char f;

void tod_reset(void)
{
  // set the tod to 0
  CIA2.tod_hour=0;
  CIA2.tod_min=0;
  CIA2.tod_sec=0;
  CIA2.tod_10=0;
}

unsigned int tod_get10(void)
{
  static unsigned char h,m,s,t;
  static unsigned int time;
  h=CIA2.tod_hour;
  m=CIA2.tod_min;
  s=CIA2.tod_sec;
  t=CIA2.tod_10;
  time=t;
  time+=(s&0xf)*10;
  time+=(s>>4)*100;
  time+=(m&0xf)*600;
  time+=(m>>4)*6000;

  return time;
}

unsigned char tod_freq(void)
{
  return f;
}

static unsigned char tod_detect_freq(void)
{
  static unsigned int cbl;

  CIA2.crb=0x40; // stop timer
  CIA2.cra=0x80; // stop timer

  // set ta to overflow every 10000 count (~= 10ms)
  CIA2.ta_lo=0x10;
  CIA2.ta_hi=0x27;
  CIA2.tb_lo=CNT;
  CIA2.tb_hi=0;

  tod_reset();

  CIA2.crb=0x41; // input from tim1 overflow, continuous, start timer
  CIA2.cra=0x81; // start timer, continuous tod 50HZ

  // wait for tod to count 1s
  while(CIA2.tod_sec==0);

  //  cal=CIA2.ta_lo;
  //  cah=CIA2.ta_hi;
  cbl=CIA2.tb_lo;
  //  cbh=CIA2.tb_hi;

  //  printf("count2 = %d %d %d %d\n",cah, cal, cbh, cbl);
  //  printf("elapsed ~= %d0ms\n",CNT-cbl);

  if((CNT-cbl)>90) return 50;
  else return 60;

}

void tod_init(unsigned char freq)
{
  if(freq==0) f=tod_detect_freq();
  else f=freq;
  
  if(freq==50) CIA2.cra|=0x80;
  else CIA2.cra&=0x7f;
}

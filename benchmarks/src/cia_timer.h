#ifndef CIA_TIMER_H
#define CIA_TIMER_H

void tod_reset(void);
unsigned int tod_get10(void);
void tod_init(unsigned char freq);
unsigned char tod_freq(void);


#endif

#include <stdio.h>

int putchar(int c) __naked
{
  (void)c;

  __asm__ ("cmp #10");
  __asm__ ("bne not_lf");
  __asm__ ("lda #0x8d");
  __asm__ ("bne skip");
  __asm__ ("not_lf:");
  __asm__ ("tax");
  __asm__ ("and #0xC0");
  __asm__ ("tay");
  __asm__ ("txa");
  __asm__ ("cpy #0x40");
  __asm__ ("bne skip");
  __asm__ ("eor #0x20");
  __asm__ ("skip:");

  //  if(c=='\n') c=13;
  //  if(c&0x40==0x40) c^=0x20;
  //  __asm__ ("lda _c");
  __asm__ ("jmp 0xffd2");
  //  return 0;
}

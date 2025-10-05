#include <stdio.h>
#include "cia_timer.h"
#include "out.h"

void benchmark(void);
void benchmark_name(void);
unsigned char benchmark_check(void);

#ifdef KICKC
#define MAIN void main()
#else
// #define MAIN int main(int argc, char *argv[])
#define MAIN int main()
#endif

#if defined(VICETRAP)
static volatile unsigned char *viceTrapPtr = (unsigned char *)VICETRAP;
static void (*viceTrap)(void) = (void (*)(void))VICETRAP;
#endif

MAIN
{
    unsigned int t;
    unsigned int f;

#if defined(OSCAR64) || defined(CALYPSI)
    *(unsigned char *)0xd018 = 0x17; // Set VIC-II to lower case mode
#endif

#if defined(CALYPSI) 
    *(unsigned char *)0x01 = 30; // Turn off BASIC ROM
#endif

    print("Calibrating frequency: ");
    tod_init(0);
    f = tod_freq();
    print_int((int)f, 0);
    print(" Hz" NEWLINE NEWLINE);

    benchmark_name();
    tod_reset();
    benchmark();
    t = tod_get10();

    print(NEWLINE);
    benchmark_check();

    print(NEWLINE "Total time: ");
    print_int(t / 10, 0);
    print(".");
    print_int(t % 10, 0);
    print(" s" NEWLINE);

#if defined(LLVM) || defined(VBCC)
    fflush(stdout);
#endif

    // Wait a bit to ensure a refresh before taking a screenshot
    tod_init(0);

#if defined(VICETRAP)
    // Trigger the VICE monitor to exit
    *viceTrapPtr = 0x60; // RTS opcode
    viceTrap();
#endif

#ifndef KICKC
    return 0;
#endif
}

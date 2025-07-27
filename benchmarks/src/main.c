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

#if defined(LLVM) || defined(SDCC) || defined(VBCC)
#define VICETRAP 0xc000
static volatile unsigned char *viceTrapPtr = (unsigned char *)VICETRAP;
static void (*viceTrap)(void) = (void (*)(void))VICETRAP;
#endif

MAIN
{
    unsigned int t;
    unsigned int f;

#ifdef OSCAR64
    *(unsigned char *)0xd018 = 0x17; // Set VIC-II to lower case mode
#endif

    print("Calibrating frequency: ");
    tod_init(0);
    f = tod_freq();
    print_int((int)f, 0);
    print(" Hz\n\n");

    benchmark_name();
    tod_reset();
    benchmark();
    t = tod_get10();

    print("\n");
    benchmark_check();

    print("\nTotal time: ");
    print_int(t / 10, 0);
    print(".");
    print_int(t % 10, 0);
    print(" s\n");

#if defined(LLVM) || defined(VBCC)
    fflush(stdout);
#endif

    // Wait a bit to ensure a refresh before taking a screenshot
    tod_init(0);

#if defined(LLVM) || defined(SDCC) || defined(VBCC)
    // Trigger the VICE monitor to exit
    *viceTrapPtr = 0x60; // RTS opcode
    viceTrap();
#endif

#ifndef KICKC
    return 0;
#endif
}

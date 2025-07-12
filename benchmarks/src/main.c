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

static volatile unsigned char *brk = (unsigned char *)0xc000;
static void (*func)(void) = (void (*)(void))0xc000;

MAIN
{
    unsigned int t;
    unsigned int f;

#ifdef OSCAR64
    // Toggle lower case mode on OSCAR64
    __asm {
            lda $17 // Load the value of $17 (VIC-II register)
            sta $d018 // Store back to VIC-II register
    }
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

    // allow the program to be stopped at a breakpoint to capture the output
#ifdef OSCAR64
    __asm {
        lda #$60 // Load accumulator with RTS opcode (0x60)
        sta $c000 // Store it at $c000
        jmp $c000 // Jump to $c000 to execute the BRK
    }
#else
    *brk = 0x60; // Write RTS opcode (0x60) at $c000 for return
    func();      // Jump to $c000 and execute BRK
#endif

#ifndef KICKC
    return 0;
#endif
}

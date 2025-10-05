#ifndef OUT_H
#define OUT_H

#ifdef NEWLINE_CR
#define NEWLINE "\r"
#else
#define NEWLINE "\n"
#endif

void print(const char *s);

void print_int(long n, int padLength);

void print_hex(unsigned long n, int padLength);

#endif

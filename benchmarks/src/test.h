#include <stddef.h>

#ifndef TEST_H
#define TEST_H

void begin(const char *test);

void success(void);

void failure(void);

void missing();

void disabled();

char expect(char result);

#if !defined(CC65)
char closeTo(float a, float b);

char expectCloseTo(float a, float b);
#endif

void randStr(const char *charset, char *str, size_t len);

void randCharStr(char *str, size_t len);

void randDigitStr(char *str, size_t len);

int randInt(int min, int max);

long randLong(long min, long max);

unsigned long crc32(const unsigned char *data, unsigned int length);

#endif
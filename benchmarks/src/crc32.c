#include "out.h"

unsigned long CRC32(const unsigned char *data, unsigned int length)
{
    /* CRC-32/CKSUM */
    unsigned char extract;
    unsigned int i;
    unsigned char j;
    unsigned long crc;
    //  const unsigned char *end=data+length;

    crc = 0;

    for (i = 0; i < length; i++)
    {
        //  while(data!=end) {
        extract = *data;
        crc ^= (((unsigned long)extract) << 24);

        for (j = 8; j; j--)
        {
            if ((crc & 0x80000000) != 0)
            {
                //       crc=(crc<<1)^0x04c11db7;
                crc <<= 1;
                crc ^= 0x04c11db7;
            }
            else
                crc <<= 1;
        }
        data++;
    }
    crc ^= 0xffffffff;
    return crc;
}

static unsigned long crc;

void benchmark(void)
{
    crc = CRC32((unsigned char *)0xe000, 0x2000);
}

void benchmark_name(void)
{
    print("crc32.c\n");
    print("Calculates the CRC32 of the C64 Kernal\n");
}

#define CORRECT 0xe1fa84c6

unsigned char benchmark_check(void)
{
    print("CRC32=");
    print_hex(crc);

    if (crc == CORRECT)
    {
        print(" [OK]");
        return 0;
    }
    else
    {
        print(" [FAIL] - expected ");
        print_hex(CORRECT);
    }

    print("\n");
    return 1;
}

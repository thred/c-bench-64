#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

#define DATA_SIZE 256
#define BUFFER_SIZE 16

typedef struct
{
    char name[16];
    int value;
    long timestamp;
    char flags[8];
    short count;
    char checksum;
} test_data_t;

static char *data =
    "One Poke to rule them all,\n"
    "One Poke to find them,\n"
    "One Poke to bring them all,\n"
    "And in the circuits bind them\n"
    "In MOS 6502 where the Memories lie.";

static char filename[16];

static test_data_t testData;

static void randTestData(test_data_t *testData);

static void test_fputc(void)
{
    char *ptr = data;
    FILE *fout = fopen(filename, "w");

    begin("fputc");

    if (fout == NULL)
    {
        failure();
        return;
    }

    while (*ptr)
    {
        if (fputc(*ptr++, fout) == EOF)
        {
            failure();
            fclose(fout);
            return;
        }
    }

    fclose(fout);

    success();
}

static void test_fgetc(void)
{
    int ch;
    int i = 0;
    FILE *fin = fopen(filename, "r");
    char result[DATA_SIZE];

    begin("fgetc");

    if (fin == NULL)
    {
        failure();
        return;
    }

    while ((ch = fgetc(fin)) != EOF && i < DATA_SIZE)
    {
        result[i++] = (char)ch;
    }

    result[i] = '\0';

    fclose(fin);

    expect(strcmp(result, data) == 0);
}

static void test_fputs(void)
{
    FILE *fout = fopen(filename, "w");

    begin("fputs");

    if (fout == NULL)
    {
        failure();
        return;
    }

    if (fputs(data, fout) == EOF)
    {
        failure();
        fclose(fout);
        return;
    }

    fclose(fout);

    success();
}

static void test_fgets(void)
{
    FILE *fin = fopen(filename, "r");
    char buffer[BUFFER_SIZE];
    char result[DATA_SIZE];
    char *resultPtr = result;
    int subsequentEmptyBufferReads = 0;

    memset(result, 0, sizeof(result));

    begin("fgets");

    if (fin == NULL)
    {
        failure();
        return;
    }

    while (fgets(buffer, sizeof(buffer), fin) != NULL)
    {
        if (memchr(buffer, 0, sizeof(buffer)) == NULL)
        {
            failure();
            printf(" null terminator missing");
            fclose(fin);
            return;
        }

        if (strlen(resultPtr) + strlen(buffer) >= DATA_SIZE)
        {
            failure();
            printf(" result buffer overflow");
            fclose(fin);
            return;
        }

        if (strlen(buffer) == 0)
        {
            subsequentEmptyBufferReads++;

            if (subsequentEmptyBufferReads > 128)
            {
                failure();
                printf(" read stalled");
                fclose(fin);
                return;
            }

            continue;
        }

        subsequentEmptyBufferReads = 0;
        strcat(resultPtr, buffer);
    }

    fclose(fin);

    expect(strcmp(result, data) == 0);
}

static void test_fwrite(void)
{
    FILE *fout = fopen(filename, "wb");

    begin("fwrite");

    if (fout == NULL)
    {
        failure();
        return;
    }

    if (fwrite(&testData, sizeof(test_data_t), 1, fout) != 1)
    {
        failure();
        fclose(fout);
        return;
    }

    fclose(fout);

    success();
}

static void test_fread(void)
{
    test_data_t readData;
    FILE *fin;

    begin("fread");

    fin = fopen(filename, "rb");

    if (fin == NULL)
    {
        failure();
        return;
    }

    if (fread(&readData, sizeof(test_data_t), 1, fin) != 1)
    {
        failure();
        fclose(fin);
        return;
    }

    fclose(fin);

    expect(memcmp(&testData, &readData, sizeof(test_data_t)) == 0);
}

void test(void)
{
    randCharStr(filename, sizeof(filename));

    test_fputc();
    test_fgetc();

    randCharStr(filename, sizeof(filename));

    test_fputs();
    test_fgets();

    randCharStr(filename, sizeof(filename));
    randTestData(&testData);

    test_fwrite();
    test_fread();
}

static void randTestData(test_data_t *testData)
{
    randCharStr(testData->name, sizeof(testData->name));
    testData->value = randInt(-1000, 1000);
    testData->timestamp = randLong(0, 1000000);
    randCharStr(testData->flags, sizeof(testData->flags));
    testData->count = (short)randInt(-100, 100);
    testData->checksum = (char)randInt(0, 255);
}

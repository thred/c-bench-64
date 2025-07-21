#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
#include <math.h>
#endif

#include "test.h"

#define PI 3.141592653

static void test_floor(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 3.7;
    float result;

    begin("floor");

    result = floor(value);
    expectCloseTo(result, 3.0);

    value = -3.7;
    result = floor(value);
    expectCloseTo(result, -4.0);

    value = 0.0;
    result = floor(value);
    expectCloseTo(result, 0.0);
#else
    begin("floor");
    missing();
#endif
}

static void test_ceil(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 3.7;
    float result;

    begin("ceil");

    result = ceil(value);
    expectCloseTo(result, 4.0);

    value = -3.7;
    result = ceil(value);
    expectCloseTo(result, -3.0);

    value = 0.0;
    result = ceil(value);
    expectCloseTo(result, 0.0);
#else
    begin("ceil");
    missing();
#endif
}

static void test_cos(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float angle = 0.0;
    float result;

    begin("cos");

    result = cos(angle);
    expectCloseTo(result, 1.0);

    angle = PI / 2;
    result = cos(angle);
    expectCloseTo(result, 0.0);

    angle = PI;
    result = cos(angle);
    expectCloseTo(result, -1.0);

    angle = (3 * PI) / 2;
    result = cos(angle);
    expectCloseTo(result, 0.0);

    angle = 2 * PI;
    result = cos(angle);
    expectCloseTo(result, 1.0);
#else
    begin("cos");
    missing();
#endif
}

static void test_sin(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float angle = 0.0;
    float result;

    begin("sin");

    result = sin(angle);
    expectCloseTo(result, 0.0);

    angle = PI / 2;
    result = sin(angle);
    expectCloseTo(result, 1.0);

    angle = PI;
    result = sin(angle);
    expectCloseTo(result, 0.0);

    angle = (3 * PI) / 2;
    result = sin(angle);
    expectCloseTo(result, -1.0);

    angle = 2 * PI;
    result = sin(angle);
    expectCloseTo(result, 0.0);
#else
    begin("sin");
    missing();
#endif
}

static void test_tan(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float angle = 0.0;
    float result;

    begin("tan");

    result = tan(angle);
    expectCloseTo(result, 0.0);

    angle = PI / 4;
    result = tan(angle);
    expectCloseTo(result, 1.0);

    angle = PI;
    result = tan(angle);
    expectCloseTo(result, 0.0);

    angle = 2 * PI;
    result = tan(angle);
    expectCloseTo(result, 0.0);
#else
    begin("tan");
    missing();
#endif
}

static void test_asin(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 0.0;
    float result;

    begin("asin");

    result = asin(value);
    expectCloseTo(result, 0.0);

    value = 1.0;
    result = asin(value);
    expectCloseTo(result, PI / 2);

    value = -1.0;
    result = asin(value);
    expectCloseTo(result, -PI / 2);

    value = 0.5;
    result = asin(value);
    expectCloseTo(result, PI / 6);
#else
    begin("asin");
    missing();
#endif
}

static void test_acos(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 1.0;
    float result;

    begin("acos");

    result = acos(value);
    expectCloseTo(result, 0.0);

    value = 0.0;
    result = acos(value);
    expectCloseTo(result, PI / 2);

    value = -1.0;
    result = acos(value);
    expectCloseTo(result, PI);

    value = 0.5;
    result = acos(value);
    expectCloseTo(result, PI / 3);
#else
    begin("acos");
    missing();
#endif
}

static void test_atan(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 0.0;
    float result;

    begin("atan");

    result = atan(value);
    expectCloseTo(result, 0.0);

    value = 1.0;
    result = atan(value);
    expectCloseTo(result, PI / 4);

    value = -1.0;
    result = atan(value);
    expectCloseTo(result, -PI / 4);
#else
    begin("atan");
    missing();
#endif
}

static void test_atan2(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float y, x;
    float result;

    begin("atan2");

    y = 0.0;
    x = 1.0;
    result = atan2(y, x);
    expectCloseTo(result, 0.0);

    y = 1.0;
    x = 1.0;
    result = atan2(y, x);
    expectCloseTo(result, PI / 4);

    y = 1.0;
    x = 0.0;
    result = atan2(y, x);
    expectCloseTo(result, PI / 2);

    y = 0.0;
    x = -1.0;
    result = atan2(y, x);
    expectCloseTo(result, PI);

    y = -1.0;
    x = 0.0;
    result = atan2(y, x);
    expectCloseTo(result, -PI / 2);
#else
    begin("atan2");
    missing();
#endif
}

static void test_exp(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 0.0;
    float result;

    begin("exp");

    result = exp(value);
    expectCloseTo(result, 1.0);

    value = 1.0;
    result = exp(value);
    expectCloseTo(result, 2.71828);

    value = 2.0;
    result = exp(value);
    expectCloseTo(result, 7.38906);

    value = -1.0;
    result = exp(value);
    expectCloseTo(result, 0.36788);
#else
    begin("exp");
    missing();
#endif
}

static void test_log(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 1.0;
    float result;

    begin("log");

    result = log(value);
    expectCloseTo(result, 0.0);

    value = 2.71828;
    result = log(value);
    expectCloseTo(result, 1.0);

    value = 7.38906;
    result = log(value);
    expectCloseTo(result, 2.0);

    value = 0.36788;
    result = log(value);
    expectCloseTo(result, -1.0);
#else
    begin("log");
    missing();
#endif
}

static void test_log10(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value = 1.0;
    float result;

    begin("log10");

    result = log10(value);
    expectCloseTo(result, 0.0);

    value = 10.0;
    result = log10(value);
    expectCloseTo(result, 1.0);

    value = 100.0;
    result = log10(value);
    expectCloseTo(result, 2.0);

    value = 0.1;
    result = log10(value);
    expectCloseTo(result, -1.0);
#else
    begin("log10");
    missing();
#endif
}

static void test_pow(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float base, exponent;
    float result;

    begin("pow");

    base = 2.0;
    exponent = 3.0;
    result = pow(base, exponent);
    expectCloseTo(result, 8.0);

    base = 5.0;
    exponent = 2.0;
    result = pow(base, exponent);
    expectCloseTo(result, 25.0);

    base = 10.0;
    exponent = 0.0;
    result = pow(base, exponent);
    expectCloseTo(result, 1.0);

    base = 4.0;
    exponent = 0.5;
    result = pow(base, exponent);
    expectCloseTo(result, 2.0);

    base = 2.0;
    exponent = -2.0;
    result = pow(base, exponent);
    expectCloseTo(result, 0.25);
#else
    begin("pow");
    missing();
#endif
}

static void test_sqrt(void)
{
#if !defined(CC65) && !defined(LLVM) && !defined(VBCC)
    float value;
    float result;

    begin("sqrt");

    value = 0.0;
    result = sqrt(value);
    expectCloseTo(result, 0.0);

    value = 1.0;
    result = sqrt(value);
    expectCloseTo(result, 1.0);

    value = 4.0;
    result = sqrt(value);
    expectCloseTo(result, 2.0);

    value = 9.0;
    result = sqrt(value);
    expectCloseTo(result, 3.0);

    value = 16.0;
    result = sqrt(value);
    expectCloseTo(result, 4.0);

    value = 2.0;
    result = sqrt(value);
    expectCloseTo(result, 1.41421);
#else
    begin("sqrt");
    missing();
#endif
}

void test(void)
{
    test_floor();
    test_ceil();

    test_cos();
    test_sin();
    test_tan();
    test_acos();
    test_asin();
    test_atan();
    test_atan2();

    test_exp();
    test_log();
    test_log10();

    test_pow();
    test_sqrt();
}

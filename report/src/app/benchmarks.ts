export type BenchmarkKey =
    | "aes256"
    | "crc8"
    | "crc16"
    | "crc32"
    | "dhrystone"
    | "fact"
    | "pi"
    | "pow"
    | "puff2"
    | "sieve"
    | "sieve_bit"
    | "test_math"
    | "test_stdio_file"
    | "test_stdio_print"
    | "test_stdio_scan"
    | "test_stdlib"
    | "test_string";

export const benchmarkKeys: readonly BenchmarkKey[] = [
    "aes256",
    "crc8",
    "crc16",
    "crc32",
    "dhrystone",
    "fact",
    "pi",
    "pow",
    "puff2",
    "sieve",
    "sieve_bit",
    "test_math",
    "test_stdio_file",
    "test_stdio_print",
    "test_stdio_scan",
    "test_stdlib",
    "test_string",
];

export type TestKey = "math.h" | "stdio.h" | "stdlib.h" | "string.h";

export const testKeys: readonly TestKey[] = ["math.h", "stdio.h", "stdlib.h", "string.h"];

export type CompilerKey = "cc65" | "kickc" | "llvm" | "o64" | "sdcc" | "vbcc";

export const compilerKeys: readonly CompilerKey[] = ["cc65", "kickc", "llvm", "o64", "sdcc", "vbcc"];

export const activeCompilerKeys: readonly CompilerKey[] = ["cc65", "llvm", "o64", "sdcc", "vbcc"];

export interface Benchmark {
    key: BenchmarkKey | "combined";
    name: string;
    shortName: string;
    description: string;
    note?: string;
    author?: string;
    url?: string;
    footnotes?: string[];
    initiallyDisabled?: boolean;
}

export const benchmarks: { [key in BenchmarkKey]: Benchmark } = {
    aes256: {
        key: "aes256",
        name: "AES-256",
        shortName: "AES-256",
        description:
            "Encrypts the C64 kernel using the AES-256 algorithm. Stresses array access and bitwise operations.",
        url: "benchmarks/src/aes256.c",
    },
    crc8: {
        key: "crc8",
        name: "CRC-8",
        shortName: "CRC-8",
        description: "Computes the 8-bit CRC of the C64 kernel ROM. Stresses logical operations on the char type.",
        url: "benchmarks/src/crc8.c",
    },
    crc16: {
        key: "crc16",
        name: "CRC-16",
        shortName: "CRC-16",
        description: "Computes the 16-bit CRC of the C64 kernel ROM. Stresses logical operations on the int type.",
        url: "benchmarks/src/crc16.c",
    },
    crc32: {
        key: "crc32",
        name: "CRC-32",
        shortName: "CRC-32",
        description: "Computes the 32-bit CRC of the C64 kernel ROM. Stresses logical operations on the long type.",
        url: "benchmarks/src/crc32.c",
    },
    dhrystone: {
        key: "dhrystone",
        name: "Dhrystone",
        shortName: "Dhrystone",
        description:
            "Dhrystone (version 2.1) is a classic computer benchmark. Its performance depends heavily on the quality of standard library routines (memcpy, strcpy, strcmp). It is not well suited for highly optimizing compilers.",
        author: "Reinhold P. Weicker",
        url: "benchmarks/src/dhrystone.c",
    },
    fact: {
        key: "fact",
        name: "Factorial",
        shortName: "Factorial",
        description:
            "Calculates the factorial of a number using naive recursion. Stresses recursion and the use of local stack variables.",
        footnotes: [
            "Oscar64 solves this benchmark at compile time, resulting in an execution time of zero seconds. LLVM can do the same, but only with -O3, which cannot be activated because some binaries would grow too large. Still, it demonstrates the power of the optimizers. Note, however, that this is a rather rare and lucky event.",
        ],
        url: "benchmarks/src/fact.c",
    },
    pi: {
        key: "pi",
        name: "Pi Calculation",
        shortName: "Pi",
        description: "Calculates 160 digits of Pi. Stresses integer multiplication, division, loops, and array access.",
        url: "benchmarks/src/pi.c",
    },
    pow: {
        key: "pow",
        name: "Power Calculation",
        shortName: "Pow",
        description: "Performs multiple floating-point exponentiation calculations.",
        footnotes: [
            "The expected output may differ due to the various floating-point implementations and representations of `float` used by the compilers.",
        ],
        url: "benchmarks/src/pow.c",
    },
    puff2: {
        key: "puff2",
        name: "zlib Compression",
        shortName: "zlib",
        description:
            "Decompresses zlib-compressed data. Stresses array access and bit operations. Most tested compilers struggled to generate a working executable.",
        author: "Copyright (C) 2002-2013 Mark Adler",
        url: "benchmarks/src/puff2.c",
    },
    sieve: {
        key: "sieve",
        name: "Sieve of Eratosthenes",
        shortName: "Sieve",
        description:
            "Calculates prime numbers using the Sieve of Eratosthenes algorithm with a byte array. Stresses loops and array access.",
        url: "benchmarks/src/sieve.c",
    },
    sieve_bit: {
        key: "sieve_bit",
        name: "Sieve of Eratosthenes (Bit)",
        shortName: "Sieve (Bit)",
        description:
            "Calculates prime numbers using the Sieve of Eratosthenes algorithm with a bitfield to conserve memory. Stresses loops, array access, and logical operations.",
        url: "benchmarks/src/sieve_bit.c",
    },
    test_math: {
        key: "test_math",
        name: "Math Library",
        shortName: "math.h",
        description: "Test of the math library functions.",
        note: "This is a test, not a benchmark! The results are not representative (but interesting nonetheless).",
        url: "benchmarks/src/test_math.c",
        initiallyDisabled: true,
    },
    test_stdio_file: {
        key: "test_stdio_file",
        name: "Standard I/O Library, File Operations",
        shortName: "stdio.h (File)",
        description: "Test of the standard I/O file library functions.",
        note: "This is a test, not a benchmark! The results are not representative (but interesting nonetheless).",
        url: "benchmarks/src/test_stdio_file.c",
        initiallyDisabled: true,
    },
    test_stdio_print: {
        key: "test_stdio_print",
        name: "Standard I/O Library, Print Functions",
        shortName: "stdio.h (Print)",
        description: "Test of the standard I/O print library functions.",
        note: "This is a test, not a benchmark! The results are not representative (but interesting nonetheless).",
        url: "benchmarks/src/test_stdio_print.c",
        initiallyDisabled: true,
    },
    test_stdio_scan: {
        key: "test_stdio_scan",
        name: "Standard I/O Library, Scan Functions",
        shortName: "stdio.h (Scan)",
        description: "Test of the standard I/O scan library functions.",
        note: "This is a test, not a benchmark! The results are not representative (but interesting nonetheless).",
        url: "benchmarks/src/test_stdio_scan.c",
        initiallyDisabled: true,
    },
    test_stdlib: {
        key: "test_stdlib",
        name: "Standard Library",
        shortName: "stdlib.h",
        description: "Test of the standard library functions.",
        note: "This is a test, not a benchmark! The results are not representative (but interesting nonetheless).",
        url: "benchmarks/src/test_stdlib.c",
        initiallyDisabled: true,
    },
    test_string: {
        key: "test_string",
        name: "String Library",
        shortName: "string.h",
        description: "Test of the string library functions.",
        note: "This is a test, not a benchmark! The results are not representative (but interesting nonetheless).",
        url: "benchmarks/src/test_string.c",
        initiallyDisabled: true,
    },
};

export interface TestDef {
    key: TestKey;
    name: string;
}

export const testDefs: { [key in TestKey]: TestDef } = {
    "math.h": {
        key: "math.h",
        name: "Math Library",
    },
    "stdio.h": {
        key: "stdio.h",
        name: "Standard I/O Library",
    },
    "stdlib.h": {
        key: "stdlib.h",
        name: "Standard Library",
    },
    "string.h": {
        key: "string.h",
        name: "String Library",
    },
};

export type Optimization = "none" | "size" | "performance";

export interface Config {
    key: string;
    name: string;
    compilerKey: CompilerKey;
    description: string;
    optimization: Optimization;
    color: string;
}

export interface ConfigResult {
    prgName: string | null;
    size: number | null;
    time: number | null;
    status: Status | null;
    output: string | null;
    screenshot?: string;
}

export interface TestSource {
    size: number;
    output?: string;
}

export type TestSources = { [key: string]: TestSource };

export type Supported =
    | "yes"
    | "no"
    | "partial"
    | "unknown"
    | string
    | {
          supported: "yes" | "no" | "partial" | "unknown";
          note?: string;
          url?: string;
      };

export type Status = "pass" | "fail" | "unsupported" | "disabled" | "unknown";

export interface Compiler {
    key: CompilerKey;
    name: string;
    description: string[];
    homepage: string;
    license: string;
    features: {
        floatingPoint: Supported;
        inlineAssembly: Supported;
        overlays: Supported;
        fileIO: Supported;
        printf: Supported;
        recursiveFunctions: Supported;
        interruptSupport: Supported;
        maintained: Supported;
    };
    pros?: string[];
    neutral?: string[];
    cons?: string[];
    runtimeSupport: string[];
    wip?: boolean;
    warning?: string;
    version: string;
    date: Date;
    configs: Config[];
    results: { [configKey: string]: ConfigResults };
    tests: { [configKey: string]: TestKeyMethodTestResultsMap };
    testSources: TestSources;
}

export type CompilersByKey = { [key in CompilerKey]?: Compiler };

export type ConfigsByKey = { [key: string]: Config };

export interface BenchmarkResult extends Config {
    size: number;
    time: number;
    status: Status;
    output?: string;
    screenshot?: string;
}

export type BenchmarkResults = { [configKey: string]: BenchmarkResult };

export interface BenchmarkWithResults extends Benchmark {
    results: BenchmarkResults;
}

export type BenchmarkWithResultsMap = {
    [key in BenchmarkKey]: BenchmarkWithResults;
};

export type BenchmarkSummary = {
    [benchmarkKey in BenchmarkKey]?: {
        [configKey: string]: BenchmarkSummaryItem;
    };
};

export interface BenchmarkSummaryItem {
    configKey?: string;
    value: number;
    status: Status;
    lowestValue?: boolean;
    hightestValue?: boolean;
    output?: string;
    screenshot?: string;
}

export type ConfigResults = { [key in BenchmarkKey]: ConfigResult };

export type MethodTestResultsMap = { [method: string]: TestResult };

export type TestKeyMethodTestResultsMap = { [key in TestKey]: MethodTestResultsMap };

export interface TestResult {
    status: Status;
    prgName?: string;
}

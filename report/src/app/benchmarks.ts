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
    | "sieve_bit";

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
];

export type CompilerKey = "cc65" | "kickc" | "llvm" | "o64" | "sdcc" | "vbcc";

export const compilerKeys: readonly CompilerKey[] = ["cc65", "kickc", "llvm", "o64", "sdcc", "vbcc"];

export const activeCompilerKeys: readonly CompilerKey[] = ["cc65", "llvm", "o64", "vbcc"];

export interface Benchmark {
    key: BenchmarkKey | "combined";
    name: string;
    shortName: string;
    description: string;
    note?: string;
    author?: string;
    url?: string;
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
        description: "Calculates the 8-bit CRC of the C64 kernel ROM. Stresses logical operations on the char type.",
        url: "benchmarks/src/crc8.c",
    },
    crc16: {
        key: "crc16",
        name: "CRC-16",
        shortName: "CRC-16",
        description: "Calculates the 16-bit CRC of the C64 kernel ROM. Stresses logical operations on the int type.",
        url: "benchmarks/src/crc16.c",
    },
    crc32: {
        key: "crc32",
        name: "CRC-32",
        shortName: "CRC-32",
        description: "Calculates the 32-bit CRC of the C64 kernel ROM. Stresses logical operations on the long type.",
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
            "Calculates the factorial of a number using naive recursion. Stresses recursion and use of local stack variables.",
        note: "LLVM-mos's optimizer computes the 1000 iterations of this benchmark at compile time. This is not a mistake; it really is that fast.",
        url: "benchmarks/src/fact.c",
    },
    pi: {
        key: "pi",
        name: "Pi Calculation",
        shortName: "Pi",
        description: "Calculates 160 digits of PI. Stresses integer multiplication, division, loops, and array access.",
        url: "benchmarks/src/pi.c",
    },
    pow: {
        key: "pow",
        name: "Power Calculation",
        shortName: "Pow",
        description: "Performs multiple floating-point exponentiation calculations.",
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
};

export interface Configuration {
    name: string;
    compilerKey: CompilerKey;
    description: string;
    optimization: "performance" | "size" | "none";
    color: string;
}

export interface ConfigurationResult {
    size: number | null;
    time: number | null;
    status: Status | null;
    output: string | null;
}

export type ConfigurationResults = { [key in BenchmarkKey]: ConfigurationResult };

export type Supported = "yes" | "no" | "partial" | "unknown" | string;

export type Status = "pass" | "fail" | "unknown";

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
    libaries: {
        assert: Supported;
        complex: Supported;
        conio: Supported;
        ctype: Supported;
        errno: Supported;
        fenv: Supported;
        float: Supported;
        inttypes: Supported;
        iso646: Supported;
        limits: Supported;
        locale: Supported;
        math: Supported;
        setjmp: Supported;
        signal: Supported;
        stdalign: Supported;
        stdarg: Supported;
        stdatomic: Supported;
        stdbit: Supported;
        stdbool: Supported;
        stddef: Supported;
        stdint: Supported;
        stdio: Supported;
        stdlib: Supported;
        stdnoreturn: Supported;
        string: Supported;
        tgmath: Supported;
        threads: Supported;
        time: Supported;
        uchar: Supported;
        wchar: Supported;
        wctype: Supported;
    };
    pros: string[];
    cons: string[];
    runtimeSupport: string[];
    wip?: boolean;
    warning?: string;
    version: string;
    date: Date;
    configurations: { [configurationkey: string]: Configuration };
    results: { [configurationKey: string]: ConfigurationResults };
}

export type Compilers = { [key in CompilerKey]?: Compiler };

export interface BenchmarkResult extends Configuration {
    size: number;
    time: number;
    status: Status;
    output?: string;
}

export type BenchmarkResults = { [configurationKey: string]: BenchmarkResult };

export interface BenchmarkWithResults extends Benchmark {
    results: BenchmarkResults;
}

export type BenchmarkWithResultsMap = {
    [key in BenchmarkKey]: BenchmarkWithResults;
};

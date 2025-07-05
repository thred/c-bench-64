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

export interface Benchmark {
    key: BenchmarkKey | "combined";
    name: string;
    shortName: string;
    description: string;
    author?: string;
    url?: string;
}

export const benchmarks: { [key in BenchmarkKey]: Benchmark } = {
    aes256: {
        key: "aes256",
        name: "AES-256",
        shortName: "AES-256",
        description:
            "This benchmark encrypts the C64 kernel using the AES-256 algorithm. It stresses array access and bitwise operations.",
        url: "benchmarks/src/aes256.c",
    },
    crc8: {
        key: "crc8",
        name: "CRC-8",
        shortName: "CRC-8",
        description:
            "This benchmark computes the 8-bit CRC of the C64 kernel ROM. It stresses logical operations on the char type.",
        url: "benchmarks/src/crc8.c",
    },
    crc16: {
        key: "crc16",
        name: "CRC-16",
        shortName: "CRC-16",
        description:
            "This benchmark computes the 16-bit CRC of the C64 kernel ROM. It stresses logical operations on the int type.",
        url: "benchmarks/src/crc16.c",
    },
    crc32: {
        key: "crc32",
        name: "CRC-32",
        shortName: "CRC-32",
        description:
            "This benchmark computes the 32-bit CRC of the C64 kernel ROM. It stresses logical operations on the long type.",
        url: "benchmarks/src/crc32.c",
    },
    dhrystone: {
        key: "dhrystone",
        name: "Dhrystone",
        shortName: "Dhrystone",
        description:
            "Dhrystone (version 2.1) is a classic computer benchmark. Its performance largely depends on the quality of some standard library routines (memcpy, strcpy, strcmp). The benchmark is not very well suited for highly optimizing compilers.",
        author: "Reinhold P. Weicker",
        url: "benchmarks/src/dhrystone.c",
    },
    fact: {
        key: "fact",
        name: "Factorial",
        shortName: "Factorial",
        description:
            "This benchmark is a naive implementation for calculating the factorial of a number using recursive function calls. It stresses recursion and the use of local variables on the stack.",
        url: "benchmarks/src/fact.c",
    },
    pi: {
        key: "pi",
        name: "Pi Calculation",
        shortName: "Pi",
        description:
            "This benchmark computes 160 digits of PI. It primarily stresses integer multiplication and division, as well as loops and array access.",
        url: "benchmarks/src/pi.c",
    },
    pow: {
        key: "pow",
        name: "Power Calculation",
        shortName: "Pow",
        description: "This benchmark performs multiple calculations of floating point exponentials.",
        url: "benchmarks/src/pow.c",
    },
    puff2: {
        key: "puff2",
        name: "zlib Compression",
        shortName: "zlib",
        description:
            "This benchmark decompresses zlib-compressed data. It stresses array access and bit operations. Most of the tested compilers had trouble generating a working executable.",
        author: "Copyright (C) 2002-2013 Mark Adler",
        url: "benchmarks/src/puff2.c",
    },
    sieve: {
        key: "sieve",
        name: "Sieve of Eratosthenes",
        shortName: "Sieve",
        description:
            "This benchmark computes prime numbers using the Sieve of Eratosthenes algorithm. The algorithm uses an array of bytes to mark non-primes. It stresses loops and array access operations.",
        url: "benchmarks/src/sieve.c",
    },
    sieve_bit: {
        key: "sieve_bit",
        name: "Sieve of Eratosthenes (Bit)",
        shortName: "Sieve (Bit)",
        description:
            "This benchmark computes prime numbers using the Sieve of Eratosthenes algorithm. The algorithm uses a bitfield to conserve memory. It stresses loops, array access and logical operations.",
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
    prgSize: number | null;
    totalTime: number | null;
    buildTime: Date | null;
    buildVersion: string | null;
}

export type ConfigurationResults = { [key in BenchmarkKey]: ConfigurationResult };

export interface Compiler {
    key: CompilerKey;
    name: string;
    description: string[];
    homepage: string;
    pros: string[];
    cons: string[];
    wip?: boolean;
    configurations: { [configurationkey: string]: Configuration };
    results: { [configurationKey: string]: ConfigurationResults };
}

export type Compilers = { [key in CompilerKey]?: Compiler };

export interface BenchmarkResult extends Configuration {
    size: number;
    time: number;
}

export type BenchmarkResults = { [configurationKey: string]: BenchmarkResult };

export interface BenchmarkWithResults extends Benchmark {
    results: BenchmarkResults;
}

export type BenchmarkWithResultsMap = {
    [key in BenchmarkKey]: BenchmarkWithResults;
};

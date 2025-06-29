# Benchmarks for MOS6502 C-Compilers

The source for the initial benchmarks is: https://web.archive.org/web/20250124180056/https://gglabs.us/node/2293  
Submitted by GG on Sat, 03/12/2022 - 01:11

> GG, please contact me to discuss licensing issues regarding your benchmarks!

Since the original web page is no longer accessible, I am attempting to preserve the information and make the results reproducible and more accessible.

## Included Compilers

-   [CC65](https://cc65.github.io/)
-   [KickC](https://gitlab.com/camelot/kickc) - currently disabled because of serious compatibility issues
-   [llvm-mos](https://llvm-mos.org)
-   [Oscar64](https://github.com/drmortalwombat/oscar64)
-   [sdcc](https://sdcc.sourceforge.net/) - currently disabled because of build errors
-   [vbcc](http://www.compilers.de/vbcc.html)

## Prepare Build

### Linux (RedHat)

#### Install "make":

```
sudo dnf install make
```

#### Install "viceemu":

https://flathub.org/apps/net.sf.VICE

## Build and Execute Benchmarks

The benchmark for each compiler contains specific instructions to prepare the build for the tested compiler.
The compilers are usually installed into a subfolder to avoid problems with existing local installations.

-   [README.md for CC65](benchmarks/cc65/README.md)
-   [README.md for KickC](benchmarks/kickc/README.md) - current disabled because of serious build problems
-   [README.md for llvm-mos](benchmarks/llvm-mos/README.md)
-   [README.md for Oscar64](benchmarks/oscar64/README.md)
-   [README.md for sdcc](benchmarks/sdcc/README.md)
-   [README.md for vbcc](benchmarks/vbcc/README.md)

Benchmarks can be built and executed using:

```
make -C benchmarks all
```

Run all benchmarks for a specific compiler:

```
make -C benchmarks all INCLUDE=cc65p,cc65s
```

The available keys are: `cc65p`, `cc65s`, `kickc`, `llvmp`, `llvms`, `o64p`, `o64s`, `sdccp`, `sdccs`, `vbccp`, `vbccs`.

Run individual benchmarks by their name:

```
make -C benchmarks crc8
```

Available benchmarks are: `aes256`, `crc8`, `crc16`, `crc32`, `dhrystone`, `fact`, `pi`, `pow`, `puff2`, `sieve`, `sieve-bit`.

## Build and Execute Reports

### Prepare

You need to install [NodeJS and NPM](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm).

In the directory `reports` execute `npm ci`.

### Updating Performance Reports

Build and execute all benchmarks before aggregating the results!

To collect the results from all benchmarks, execute `npm run aggregate`. It will update the `.json` files in the directory `reports/public`.

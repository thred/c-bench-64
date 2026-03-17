# Benchmarks for MOS6502 C-Compilers

The source for the initial benchmarks comes from: https://web.archive.org/web/20250124180056/https://gglabs.us/node/2293  
Submitted by GG on Sat, 03/12/2022 - 01:11

[Thanks for sharing your benchmarks, GG!](https://github.com/thred/c-bench-64/issues/9) Without your work, this project would not have been possible.

## Included Compilers

- [Calypsi](https://www.calypsi.cc/)
- [CC65](https://cc65.github.io/)
- [llvm-mos](https://llvm-mos.org)
- [Oscar64](https://github.com/drmortalwombat/oscar64)
- [sdcc](https://sdcc.sourceforge.net/)
- [vbcc](http://www.compilers.de/vbcc.html)

## Benchmarks

### Prepare Environment

#### Linux (RedHat)

##### Install "make":

```sh
sudo dnf install make
```

##### Install "viceemu":

Follow the documentation on: https://flathub.org/apps/net.sf.VICE

### Building and Executing the Benchmarks

The benchmark for each compiler contains specific instructions to prepare the build for the tested compiler.

The compilers are usually installed into a subfolder to avoid problems with existing local installations.

- [README.md for Calypsi](benchmarks/calypsi/README.md)
- [README.md for CC65](benchmarks/cc65/README.md)
- [README.md for llvm-mos](benchmarks/llvm-mos/README.md)
- [README.md for Oscar64](benchmarks/oscar64/README.md)
- [README.md for sdcc](benchmarks/sdcc/README.md)
- [README.md for vbcc](benchmarks/vbcc/README.md)

Benchmarks can be built and executed using:

```sh
make -C benchmarks all
```

Run all benchmarks for a specific compiler:

```sh
make -C benchmarks all INCLUDE=cc65
```

The available keys are: `calypsi`, `cc65`, `llvm`, `o64`, `sdcc`, `vbcc`.

Run individual benchmarks by their name:

```sh
make -C benchmarks crc8
```

Available benchmarks are: `aes256`, `crc8`, `crc16`, `crc32`, `dhrystone`, `fact`, `pi`, `pow`, `puff2`, `sieve`, `sieve-bit`.

## Reports

The reports consist of an interactive webpage built with [Angular](https://angular.dev).

### Prepare Environment

#### Linux (RedHat)

##### Install "nodejs" and "npm":

You need to install [NodeJS and NPM](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm).

```sh
sudo dnf install nodejs npm
```

### Building and Excecuting the Reports

> **Warning:**  
> Execute the following commands in the `reports` directory!

Download and install all dependencies:

```sh
npm ci
```

Start the application using:

```sh
npm start
```

### Updating Performance Reports

> **Warning:**  
> Build and execute all benchmarks before aggregating the results!

To collect the results from all benchmarks, execute:

```sh
npm run aggregate
```

It will update the `.json` files in the `reports/public` directory.

# Benchmarks for MOS6502 C-Compilers

The source for the initial benchmarks comes from: https://web.archive.org/web/20250124180056/https://gglabs.us/node/2293  
Submitted by GG on Sat, 03/12/2022 - 01:11

> GG, please contact me to discuss licensing issues regarding your benchmarks!

Since the original web page is no longer accessible, I am attempting to preserve the information and make the results reproducible and more accessible.

## Included Compilers

-   [CC65](https://cc65.github.io/)
-   [KickC](https://gitlab.com/camelot/kickc)
-   [llvm-mos](https://llvm-mos.org)
-   [Oscar64](https://github.com/drmortalwombat/oscar64)
-   [sdcc](https://sdcc.sourceforge.net/)
-   [vbcc](http://www.compilers.de/vbcc.html)

## Benchmarks

Documentation for Windows is coming, soon.

### Prepare Your Envrionment

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

-   [README.md for CC65](benchmarks/cc65/README.md)
-   [README.md for KickC](benchmarks/kickc/README.md)
-   [README.md for llvm-mos](benchmarks/llvm-mos/README.md)
-   [README.md for Oscar64](benchmarks/oscar64/README.md)
-   [README.md for sdcc](benchmarks/sdcc/README.md)
-   [README.md for vbcc](benchmarks/vbcc/README.md)

Benchmarks can be built and executed using:

```sh
make -C benchmarks all
```

Run all benchmarks for a specific compiler:

```sh
make -C benchmarks all INCLUDE=cc65
```

The available keys are: `cc65`, `kickc`, `llvm`, `o64`, `sdcc`, `vbcc`.

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

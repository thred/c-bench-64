# SDCC - Small Device C Compiler

Homepage: https://sdcc.sourceforge.net/

## Prepare Build

Download the latest release from: https://sdcc.sourceforge.net/snap.php

WARNING: Version 4.4.0 and later fail to produce working C64 images.

Unpack the archive to this folder, in order that this `sdcc` folder contains a `sdcc` folder with
the contents of the archive.

## Execute Benchmarks

Execute the following command in the root directory of the repository:

```bash
make -C benchmarks INCLUDE=sdcc
```

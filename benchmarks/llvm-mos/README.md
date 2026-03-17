# LLVM-MOS

Homepage: https://llvm-mos.org

## Prepare Build

Download the latest release from: https://github.com/llvm-mos/llvm-mos-sdk#getting-started

Unpack the archive to this folder, so that this `llvm-mos` folder contains another `llvm-mos` folder with the contents of the archive.

## Execute Benchmarks

Execute the following command in the root directory of the repository:

```bash
make -C benchmarks INCLUDE=llvm
```

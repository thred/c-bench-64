# VBCC - portable ISO C compiler

Homepage: http://www.compilers.de/vbcc.html

## Prepare Build

Download the latest release. The release contains a directory for your OS. Extract the contents of
this directory to a directory called `vbcc` within this `vbcc` directory.

## Execute Benchmarks

Execute the following command in the root directory of the repository:

```bash
make -C benchmarks INCLUDE=vbcc
```

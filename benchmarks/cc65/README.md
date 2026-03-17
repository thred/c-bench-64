# CC65

Homepage: https://cc65.github.io/

## Prepare Build

Follow the instructions on https://cc65.github.io/getting-started.html to download the source and place it in a subdirectory called `cc65`.

### Build it with Fedora

Make sure you have the build essentials installed. In the `cc65` directory, execute the following command:

```bash
make
```

## Execute Benchmarks

Execute the following command in the root directory of the repository:

```bash
make -C benchmarks INCLUDE=cc65
```

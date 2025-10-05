#!/bin/bash

# This script updates the cc65 benchmark by pulling the latest changes from its GitHub repository
# and then recompiling it. The script assumes that the cc65 repository has already been cloned
# into the `benchmarks/cc65/cc65` directory.

cd benchmarks/cc65/cc65 || exit 1

git pull || exit 1

make || exit 1

cd ../../.. || exit 1

make -C benchmarks all INCLUDE=cc65 || exit 1

cd report

npm run aggregate || exit 1


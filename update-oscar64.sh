#!/bin/bash

# This script updates the oscar64 benchmark by pulling the latest changes from its GitHub repository
# and then recompiling it. The script assumes that the oscar64 repository has already been cloned
# into the `benchmarks/oscar64/oscar64` directory.

cd benchmarks/oscar64/oscar64 || exit 1

git pull || exit 1

make -C make || exit 1

cd ../../.. || exit 1

make -C benchmarks all INCLUDE=o64 || exit 1

cd report

npm run aggregate || exit 1


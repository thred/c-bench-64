#!/bin/bash

# This script updates the llvm-mos benchmark by downloading the latest release from GitHub
# and extracting it to the benchmarks/llvm-mos directory.

set -e

LLVM_MOS_DIR="benchmarks/llvm-mos"
DOWNLOAD_URL="https://github.com/llvm-mos/llvm-mos-sdk/releases/latest/download/llvm-mos-linux.tar.xz"

# Create directory if it doesn't exist
mkdir -p "$LLVM_MOS_DIR"

cd "$LLVM_MOS_DIR" || exit 1

echo "Downloading latest llvm-mos release..."
wget -O llvm-mos-linux.tar.xz "$DOWNLOAD_URL" || exit 1

echo "Cleaning up llvm-mos/llvm-mos directory..."
# Clean up the llvm-mos subdirectory except .gitkeep files
if [ -d "llvm-mos" ]; then
    find llvm-mos -mindepth 1 ! -name '.gitkeep' -delete 2>/dev/null || true
fi

echo "Extracting llvm-mos..."
# Create the llvm-mos subdirectory and extract there
mkdir -p llvm-mos
tar -xJf llvm-mos-linux.tar.xz -C llvm-mos --strip-components=1 || exit 1

# Clean up the downloaded archive
rm llvm-mos-linux.tar.xz

echo "llvm-mos updated successfully"

cd ../.. || exit 1

make -C benchmarks all INCLUDE=llvm || exit 1

cd report

npm run aggregate || exit 1


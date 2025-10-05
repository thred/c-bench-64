#!/bin/bash

# This script updates the SDCC benchmark by extracting a manually downloaded snapshot
# from SourceForge and extracting it to the benchmarks/sdcc directory.

set -e

SDCC_DIR="benchmarks/sdcc"

# Create directory if it doesn't exist
mkdir -p "$SDCC_DIR"

echo "=============================================="
echo "MANUAL DOWNLOAD REQUIRED"
echo "=============================================="
echo
echo "SDCC cannot be automatically downloaded from SourceForge."
echo "Please follow these steps:"
echo
echo "1. Go to: https://sdcc.sourceforge.net/snap.php"
echo "2. Download the latest Linux x64 snapshot (sdcc-*.tar.bz2)"
echo "3. Save the file to: $PWD/$SDCC_DIR/"
echo "4. The filename should be something like: sdcc-snapshot-YYYYMMDD-*-x86_64-unknown-linux2.5.tar.bz2"
echo
echo "=============================================="
echo

# Wait for user confirmation
read -p "Press ENTER when you have downloaded the SDCC snapshot to $SDCC_DIR/ directory..."

cd "$SDCC_DIR" || exit 1

# Find the downloaded SDCC snapshot
SDCC_ARCHIVE=$(find . -maxdepth 1 -name "sdcc-*.tar.bz2" -type f | head -1)

if [ -z "$SDCC_ARCHIVE" ]; then
    echo "ERROR: No SDCC snapshot found in $SDCC_DIR/"
    echo "Please make sure you downloaded a file matching: sdcc-*.tar.bz2"
    exit 1
fi

echo "Found SDCC snapshot: $SDCC_ARCHIVE"

echo "Cleaning up sdcc/sdcc directory..."
# Clean up the sdcc subdirectory except .gitkeep files
if [ -d "sdcc" ]; then
    find sdcc -mindepth 1 ! -name '.gitkeep' -delete 2>/dev/null || true
fi

echo "Extracting SDCC..."
# Create the sdcc subdirectory and extract there
mkdir -p sdcc
tar -xjf "$SDCC_ARCHIVE" -C sdcc --strip-components=1 || exit 1

# Clean up the downloaded archive
rm sdcc-*.tar.bz2

echo "SDCC updated successfully from $SDCC_ARCHIVE"

cd ../.. || exit 1

make -C benchmarks all INCLUDE=sdcc || exit 1

cd report

npm run aggregate || exit 1


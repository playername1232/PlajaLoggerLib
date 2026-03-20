#!/bin/bash
set -e

# ensure build folder exists
mkdir -p build

# cleanup build content except exports
find build -mindepth 1 \
    ! -path "build/LinuxExport" \
    ! -path "build/LinuxExport/*" \
    ! -path "build/WindowsExport" \
    ! -path "build/WindowsExport/*" \
    -exec rm -rf {} +

# build
cmake -S . -B build
cmake --build build

# export
mkdir -p build/LinuxExport

cp PlajaLogger.h build/LinuxExport/

find build -name "*.so" -not -path "build/LinuxExport/*" -not -path "build/WindowsExport/*" -exec cp {} build/LinuxExport/ \;

# cleanup build content except exports
find build -mindepth 1 \
    ! -path "build/LinuxExport" \
    ! -path "build/LinuxExport/*" \
    ! -path "build/WindowsExport" \
    ! -path "build/WindowsExport/*" \
    -exec rm -rf {} +

echo "DONE"
echo "Output:"
ls -la build/LinuxExport
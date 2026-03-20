$ErrorActionPreference = "Stop"

# ensure build folder exists
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# cleanup build content except exports
Get-ChildItem -Path "build" | Where-Object {
    $_.Name -ne "WindowsExport" -and $_.Name -ne "LinuxExport"
} | Remove-Item -Recurse -Force

# build
cmake -S . -B build
cmake --build build

# export
New-Item -ItemType Directory -Force -Path "build/WindowsExport" | Out-Null

Copy-Item "PlajaLogger.h" "build/WindowsExport/" -Force

$filesToCopy = Get-ChildItem -Path "build" -Recurse -File | Where-Object {
    $_.FullName -notlike "*\WindowsExport\*" -and
    $_.FullName -notlike "*\LinuxExport\*" -and
    ($_.Extension -eq ".dll" -or $_.Extension -eq ".lib")
}

foreach ($file in $filesToCopy) {
    Copy-Item $file.FullName "build/WindowsExport/" -Force
}

# cleanup build content except exports again
Get-ChildItem -Path "build" | Where-Object {
    $_.Name -ne "WindowsExport" -and $_.Name -ne "LinuxExport"
} | Remove-Item -Recurse -Force

Write-Host "DONE"
Write-Host "Output:"
Get-ChildItem "build/WindowsExport"